#include "constant.h"


// PID controllers for pitch and yaw
PID pitchController(h, Kp, Ki, Kd, saturation_upper, saturation_lower);
PID yawController(h, Kp, Ki, Kd, saturation_upper, saturation_lower);

// Servo controls for TVC (Thrust Vector Control)
servoTransfer pitchServoControl(pitchServoPin);
servoTransfer yawServoControl(yawServoPin);
servoTransfer motorServoControl(motorServoPin, "motor");

void setup() {
  // Set initial position of servos to neutral (0 degrees)
  // Set initial position of motor to neutral (0 degrees)
  pitchServoControl.move(0);  
  yawServoControl.move(0);
  motorServoControl.configureESC();  // Configure the ESC

  // Set initial setpoints for motor

  //WARNING ALERT: at this point the motor is moving to the setpoint, be careful with the motor 
  motorServoControl.moveMotor(180);  // Move motor to 180 degrees (Max RPM)

  delay(5000);  // Time for configuration/stabilization



}

void loop() {
  // Read the current pitch and yaw angles (replace with actual sensor reading logic)
  // float pitchAngle = ... ;
  // float yawAngle = ... ;

  // Calculate control signal for pitch and yaw using PID controllers
  u_pitch = pitchController.PID_iteration(SP_pitch, pitchAngle);  // SP - Y(t) for pitch
  u_yaw = yawController.PID_iteration(SP_yaw, yawAngle);          // SP - Y(t) for yaw

  // Move servos based on PID outputs
  pitchServoControl.move(u_pitch);  // Move pitch servo
  yawServoControl.move(u_yaw);      // Move yaw servo

  // Optional: Add a small delay if needed (remove or adjust accordingly)
  // delay(h);
}
