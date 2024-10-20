#include "servoTransfer.h"


servoTransfer::servoTransfer(int myServoPin){

  myServo.attach(myServoPin);    

}

void servoTransfer::move(float angle){

  _angle = angle;

  _angle = _angle + 90;

  myServo.write(_angle); 

}
