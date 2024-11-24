#include "constant.h"
#include "IMU.h"


// PID controllers for pitch and yaw
PID pitchController(Kp, Ki, Kd, saturation_upper, saturation_lower);
PID yawController(Kp, Ki, Kd, saturation_upper, saturation_lower);

// Servo controls for TVC (Thrust Vector Control)
servoTransfer pitchServoControl(pitchServoPin);
servoTransfer yawServoControl(yawServoPin);
servoTransfer motorServoControl(motorServoPin, "motor");

void setup() {

  Serial.begin(115200);
  // Set initial position of servos to neutral (0 degrees)
  // Set initial position of motor to neutral (0 degrees)
  pitchServoControl.move(0);  
  yawServoControl.move(0);
  motorServoControl.configureESC();  // Configure the ESC

  // Set initial setpoints for motor

  //WARNING ALERT: at this point the motor is moving to the setpoint, be careful with the motor 
  motorServoControl.moveMotor(10);  // Move motor to 180 degrees (Max RPM)

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

void loop() {

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
  //pitchServoControl.move(u_pitch);  // Move pitch servo
  //yawServoControl.move(u_yaw);      // Move yaw servo

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
