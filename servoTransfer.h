#ifndef SERVOTRANSFER_H
#define SERVOTRANSFER_H

#include <ESP32Servo.h>  // Include the ESP32Servo library

class servoTransfer {
  public:
    // Constructor
    servoTransfer(int myServoPin, String servoName = "servo");

    // Method to move the servo
    void move(float angle);

    // Method to move the motor
    void moveMotor(float angleMotor);

    // Method to configure the ESC
    void configureESC();

  private:
    Servo myServo;    // Servo object
    float _angle;     // Stores the current angle
    float _angleMotor;  // Stores the current angle for the motor
};

#endif  // SERVOTRANSFER_H