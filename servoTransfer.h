#ifndef SERVOTRANSFER_H
#define SERVOTRANSFER_H

#include <ESP32Servo.h>  // Include the ESP32Servo library


class servoTransfer {

  public:

    Servo myServo;

    servoTransfer(int myServoPin);

    void move(float angle);

  private:

  float _angle;


};


#endif 