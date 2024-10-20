#include "constant.h"
 
PID pitchController(h , Kp, Ki, Kd, saturation_upper, saturation_lower);
PID yawController(h , Kp, Ki, Kd, saturation_upper, saturation_lower);

servoTransfer pitchControler(pitchServoPin);
servoTransfer yawControler(yawServoPin);

void setup(){

  pitchControler.move(0); //initiall position of the TVC
  yawControler.move(0);   //initiall position of the TVC

  delay(5000);            //Configutation time

}

void loop(){

  // Y : read pitch
  // Y : read yaw

  u_pitch = pitchController.PID_iteration( SP_pitch, pitchAngle);  // SP - Y(t)
  u_yaw = yawController.PID_iteration( SP_yaw, yawAngle);  // SP - Y(t)

  pitchControler.move(u_pitch);   // Servo move Pitch : insert u_pitch
  yawControler.move(u_yaw); // Servo move Yaw : insert u_yaw

  //delay(h);

}