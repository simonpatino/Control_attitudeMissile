#include "PID.h" 

float h = 0.1; //Delta time between measurements 

float Kp = 1; // PID constant 
float Ki = 1; // PID constant
float Kd = 1; // PID constant

int saturation_upper = 90;  //Saturarion of the action signal
int saturation_lower = -90; //Saturarion of the action signal

int SP_pitch = 10; // Setpoint (grades)
int SP_yaw = 10; // Setpoint (grades)

float pitchAngles; // (grades)
float yawAngles;   // (grades)

float _errorsPitch[3] = {0, 0, 0}; // index[0] = f_-2 , index[1] = f_-1 , index[3] = f_0
                                   // Where f is the function value at time (x)

float _errorsYaw[3] = {0, 0, 0};   //  ndex[0] = f_-2 , index[1] = f_-1 , index[3] = f_0
                                   // Where f is the function value at time (x)

PID pitchController(h , Kp, Ki, Kd, saturation_upper, saturation_lower);
PID yawController(h , Kp, Ki, Kd, saturation_upper, saturation_lower);

void setup(){


}


void loop(){

  _errorsPitch[2] = SP_pitch - pitchAngles;
  _errorsYaw[2] = SP_yaw - yawAngles;

  pitchController.PID_iteration(_errorsPitch);
  yawController.PID_iteration(_errorsYaw);

  // Servo move Pitch
  // Servo move Yaw

  _errorsPitch[1] =  _errorsPitch[2];
  _errorsYaw[1] =  _errorsYaw[2];

  _errorsPitch[0] =  _errorsPitch[1];
  _errorsYaw[0] =  _errorsYaw[1]; 

}