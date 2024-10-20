#include "servoTransfer.h"

//Constructor
servoTransfer::servoTransfer(int myServoPin){
    myServo.attach(myServoPin);    
}

// Method to move the servo
void servoTransfer::move(float angle){
   _angle = angle;
   _angle = _angle + 90;
   myServo.write(_angle); 
}
