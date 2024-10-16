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


float PID::PID_iteration(float errors[]){

  e_proportional =  errors[2];

  if (_SW){

  e_integrative = (2*_h)*(errors[0] + 4*errors[1] + errors[2])/3;

  } else {

  e_integrative = 0;

  }

  e_derivative = (3*errors[2] - 4*errors[1] + errors[0])/(2*_h);

  u = _Kp*e_proportional + _Ki*e_integrative + _Kd*e_derivative; 

  //System of Anti-Windup

  if ( u > _saturation_upper || u < _saturation_lower ){

    _SW = false;

  } else {

    _SW = true;

  } 

  return u ;

}