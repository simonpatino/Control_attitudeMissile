#include "constant.h"
#include "IMU.h"

//MQTT libraries
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
//


// PID controllers for pitch and yaw
PID pitchController(Kp, Ki, Kd, saturation_upper, saturation_lower);
PID yawController(Kp, Ki, Kd, saturation_upper, saturation_lower);

// Servo controls for TVC (Thrust Vector Control)
servoTransfer pitchServoControl(pitchServoPin);
servoTransfer yawServoControl(yawServoPin);
servoTransfer motorServoControl(motorServoPin, "motor");

//MQTT

WiFiClient espClient;
PubSubClient client(espClient);

//MQTT SSID/Password 
const char* ssid = " ";
const char* password = " ";
const char* mqtt_server = " ";
//

void setup() {

  Serial.begin(115200);

  // MQTT - setup

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);


  // Set initial position of servos to neutral (0 degrees)
  // Set initial position of motor to neutral (0 degrees)
  pitchServoControl.move(0);  
  yawServoControl.move(0);
  motorServoControl.configureESC();  // Configure the ESC

  // Set initial setpoints for motor

  //WARNING ALERT: at this point the motor is moving to the setpoint, be careful with the motor 
  motorServoControl.moveMotor(60);  // Move motor to 180 degrees (Max RPM)

  delay(5000);  // Time for configuration/stabilization

// #####################  BNO configuration ################################

  Wire.begin(7, 6);  // Initialize I2C with GPIO 6 as SCL and GPIO 7 as SDA
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(1000);
  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);
   
// #########################################################################

}

// #########################-MQTT - WIFI - SETUP#####################################

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// #########################################################################

// #########################-MQTT - Callback - to recibe something#####################################

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  // Combine incoming message bytes into a string
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Check if the message is for the motor control topic
  if (String(topic) == "esp32/motor") {
    int motorCommand = messageTemp.toInt(); // Convert the message to an integer
    if (motorCommand == 0) {
      motorServoControl.moveMotor(0); // Stop the motor
      Serial.println("Motor turned OFF.");
    } else if (motorCommand == 1) {
      motorServoControl.moveMotor(180); // Start the motor (or adjust as needed)
      Serial.println("Motor turned ON.");
    } else {
      Serial.println("Invalid motor command. Use 0 (OFF) or 1 (ON).");
    }
  }
}

// #########################################################################

// #########################-MQTT - reconnect #####################################

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Suscripciones a tópicos
      client.subscribe("esp32/motor");
      client.subscribe("esp32/IMU");

      // No sé si esto sea necesario para públicar el estado de los servos también xd
      //client.subscribe("esp32/servo1/state"); // Nuevo tópico
      //client.subscribe("esp32/servo2/state"); // Nuevo tópico 

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// #########################################################################




void loop() {

   // Ensure MQTT connection
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read current time
  long now = millis();

  // Perform periodic tasks every 1 second
  if (now - lastMsg > 1000) {
    lastMsg = now;

    // Read IMU sensor data
    sensors_event_t event;
    bno.getEvent(&event);

    // Publish IMU orientation data to MQTT topic
    String imuData = String("X: ") + event.orientation.x +
                     " Y: " + event.orientation.y +
                     " Z: " + event.orientation.z;
    client.publish("esp32/IMU", imuData.c_str());
    Serial.print("IMU Data: ");
    Serial.println(imuData);

    // Publish servo state data to MQTT topics
    String servo1State = String("Servo 1 Angle: ") + servo1_angle;
    String servo2State = String("Servo 2 Angle: ") + servo2_angle;

    client.publish("esp32/servo1/state", String(servo1_angle).c_str());
    client.publish("esp32/servo2/state", String(servo2_angle).c_str());

    Serial.print("Servo 1 State: ");
    Serial.println(servo1_angle);

    Serial.print("Servo 2 State: ");
    Serial.println(servo2_angle);
  }

  // #############################################################################3

  // Read the current pitch and yaw angles (replace with actual sensor reading logic)
  sensors_event_t event;
  bno.getEvent(&event);

  float pitchAngle = ((float)event.orientation.y);
  float yawAngle = ((float)event.orientation.z);

  //Debug 
  //Serial.print(F("Orientation: "));
  //Serial.print(360 - (float)event.orientation.x);
  //Serial.print(F(", "));
  //Serial.print(pitchAngle);
  //Serial.print(F(", "));
  //Serial.print(yawAngle);
  //Serial.println(F(""));

  //Serial.print("Pitch Angle: ");
  //Serial.print(pitchAngle);
  //Serial.print(", Yaw Angle: ");
  //Serial.println(yawAngle);

  //Serial.print(pitchAngle);
  //Serial.print(",");
  //Serial.println(yawAngle);


  //Update the time of the PID
  times[1] =  millis();
  DeltaT = times[1] - times[0];

  pitchController._h = DeltaT;  
  yawController._h = DeltaT;

  // Calculate control signal for pitch and yaw using PID controllers
  u_pitch = pitchController.PID_iteration(SP_pitch, pitchAngle);  // SP - Y(t) for pitch
  u_yaw = yawController.PID_iteration(SP_yaw, yawAngle);          // SP - Y(t) for yaw



  Serial.print("U pitch: ");
  Serial.print(u_pitch);
  Serial.print(", U yaw: ");
  Serial.println(u_yaw);


  Serial.print(u_pitch);
  Serial.print(",");
  Serial.println(u_yaw);


  // Move servos based on PID outputs
  pitchServoControl.move(u_pitch);  // Move pitch servo
  yawServoControl.move(u_yaw);      // Move yaw servo

  //Serial.print(u_pitch);
  //Serial.println(F(""));
  //Serial.print(u_yaw);
  //Serial.println(F(""));

  // Optional: Add a small delay if needed (remove or adjust accordingly)
  //Delay();

  //Update the last time with the new time
  times[0] = times[1];

  //Serial.println(pitchController.e_proportional);
  //Serial.println(pitchController.e_derivative);
}
