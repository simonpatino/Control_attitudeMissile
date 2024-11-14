#include "PID.h" 
#include "servoTransfer.h"

float h = 0.1; //Delta time between measurements 

float Kp = 1; // PID constant 
float Ki = 1; // PID constant
float Kd = 1; // PID constant

int saturation_upper = 82;  //Saturarion of the action signal
int saturation_lower = -90; //Saturarion of the action signal

int SP_pitch = 0; // Setpoint (grades)
int SP_yaw = 0; // Setpoint (grades)

float u_pitch;  //control action
float u_yaw;  //control action

float pitchAngle; // Actual pitch (grades)
float yawAngle;   // Actual Yaw (grades)

int pitchServoPin = 20;
int yawServoPin = 21;
int motorServoPin = 19;
