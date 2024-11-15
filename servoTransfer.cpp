#include "servoTransfer.h"

//Constructor
servoTransfer::servoTransfer(int myServoPin, String servoName){

    if (servoName == "servo"){
        myServo.attach(myServoPin);
    } 
    else  if (servoName == "motor"){
        myServo.attach(myServoPin, 1000, 2000);
    }
}

// Function to move the servo
void servoTransfer::move(float angle){
   _angle = angle;
   _angle = _angle + 90;
   myServo.write(_angle); 
}

// Function to move the motor
void servoTransfer::moveMotor(float angleMotor){
   _angleMotor = angleMotor;
   myServo.write(_angleMotor);
}

// Function to configure the ESC
void servoTransfer::configureESC() {
  delay(2000);  // Wait for 2 seconds

  // Set ESC to maximum position
  myServo.write(180);  // Maximum throttle
  delay(2000);  // Wait for 2 seconds

  // Set ESC to minimum position
  myServo.write(0);  // Minimum throttle
  delay(2000);  // Wait for 2 seconds

}
