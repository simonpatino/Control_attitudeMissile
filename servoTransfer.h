#ifndef SERVOTRANSFER_H
#define SERVOTRANSFER_H

#include <ESP32Servo.h>  // Include the ESP32Servo library

class servoTransfer {
  public:
    // Constructor
    servoTransfer(int myServoPin);

    // Method to move the servo
    void move(float angle);

  private:
    Servo myServo;    // Servo object
    float _angle;     // Stores the current angle
};

#endif  // SERVOTRANSFER_H