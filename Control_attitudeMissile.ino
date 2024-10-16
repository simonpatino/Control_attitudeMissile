#include "PID.h" 

float h = 0.1; //Delta time between measurements 

float Kp = 1; // PID constant 
float Ki = 1; // PID constant
float Kd = 1; // PID constant

int saturation_upper = 90;  //Saturarion of the action signal
int saturation_lower = -90; //Saturarion of the action signal

int SP_pitch = 0; // Setpoint (grades)
int SP_yaw = 0; // Setpoint (grades)

float u_pitch;  //control action
float u_yaw;  //control action

float pitchAngle; // Actual pitch (grades)
float yawAngle;   // Actual Yaw (grades)

 
PID pitchController(h , Kp, Ki, Kd, saturation_upper, saturation_lower);
PID yawController(h , Kp, Ki, Kd, saturation_upper, saturation_lower);

void setup(){


}

void loop(){

  //read pitch
  //read yaw

  u_pitch = pitchController.PID_iteration( SP_pitch, pitchAngle);
  u_yaw = yawController.PID_iteration( SP_yaw, yawAngle);

  // Servo move Pitch : insert u_pitch
  // Servo move Yaw : insert u_yaw

  delay(h);

}