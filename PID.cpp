#include "PID.h"
#include <Arduino.h>

 PID::PID(float h, float Kp, float Ki, float Kd, float saturation_upper, float saturation_lower) {
    _h = h; // Time  between measurments
    _Kp = Kp;  // _Kp is the class member, Kp is the parameter
    _Ki = Ki;
    _Kd = Kd;
    _saturation_upper = saturation_upper;
    _saturation_lower = saturation_lower;
    _SW = true;
}

float PID::PID_iteration(float SP, float actualValue){

  errors[2] = SP - actualValue;

  //Proportional error
  e_proportional =  errors[2];

  //Integrative for trapzoidal rule and check if the system is in anti-windup

  if (_SW){
  e_integrative = (_h)*(errors[1] + errors[2])/2;
  } else {
  e_integrative = 0;
  }

  //Integrative accumulation
  e_integrative_acumulation += e_integrative;

  //Derivative of 3 points backward
  e_derivative = (3*errors[2] - 4*errors[1] + errors[0])/(2*_h);

  //Action control 
  u = _Kp*e_proportional + _Ki*e_integrative_acumulation + _Kd*e_derivative; 

  //System of Anti-Windup
  if ( u > _saturation_upper || u < _saturation_lower ){
    _SW = false;
  } else {
    _SW = true;
  } 

  //Saturation limits
  if ( u > _saturation_upper) {
    u = _saturation_upper;
  } else if (u < _saturation_lower) {
    u = _saturation_lower;
  }

  //Update errors
  errors[1] =  errors[2];
  errors[0] =  errors[1];

  return u ;

}