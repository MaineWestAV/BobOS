// Copyright (c) 2019 Maine Township High School West Autonomous
// Vehicle Competiton Team, Sebastian Moruszewicz, Ghifari Aditya,
// Danica Roque, Veronica Kleinschmidt, Daniel Jeong, and Samuel
// "Fish" Hooson. All rights reserved.

#include <QTRSensors.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define NUM_SENSORS 6            // number of sensors used
#define NUM_SAMPLES_PER_SENSOR 4 // average 4 analog samples per sensor reading
#define EMITTER_PIN 2            // emitter is controlled by digital pin 2

#define MOTOR_DURATION 1         // duration of each motor "step" (in secs)
#define MOTOR_SPEED 150          // speed of each motor
#define MOTOR_PIN_LEFT 1         // pin location of the left motor
#define MOTOR_PIN_RIGHT 3        // pin location of the right motor

QTRSensors qtr;
uint16_t sensorValues[NUM_SENSORS];

Adafruit_MotorShield shield;
Adafruit_DCMotor *motorLeft;
Adafruit_DCMotor *motorRight;

void setup()
{
  qtr.setTypeAnalog();
  // sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5}, NUM_SENSORS);
  qtr.setEmitterPin(EMITTER_PIN);

  delay(500);
  // TODO: use NeoPixel to indicate calibration phase

  for(uint16_t i = 0; i < 400; i++)
    qtr.calibrate();

  // TODO: use NeoPixel to indicate end of calibration, then turn off

  shield = Adafruit_MotorShield();

  motorLeft = shield.getMotor(MOTOR_PIN_LEFT);
  motorRight = shield.getMotor(MOTOR_PIN_RIGHT);

  shield.begin();

  motorLeft->setSpeed(MOTOR_SPEED);
  motorRight->setSpeed(MOTOR_SPEED);
}

// Forward on BOTH motors
void motor_both_forward()
{
  motorLeft->run(FORWARD);
  motorRight->run(FORWARD);

  delay(MOTOR_DURATION);

  motorLeft->run(RELEASE);
  motorRight->run(RELEASE);
}

// Reverse on BOTH motors
void motor_both_reverse()
{
  motorLeft->run(BACKWARD);
  motorRight->run(BACKWARD);

  delay(MOTOR_DURATION);

  motorLeft->run(RELEASE);
  motorRight->run(RELEASE);
}

// Forward on LEFT motor only
void motor_left_forward()
{
  motorLeft->run(FORWARD);

  delay(MOTOR_DURATION);

  motorLeft->run(RELEASE);
}

// Forward on RIGHT motor only
void motor_right_forward()
{
  motorRight->run(FORWARD);

  delay(MOTOR_DURATION);

  motorRight->run(RELEASE);
}

void loop()
{
  // BEGIN Nav Algorithm pseudocode
  // read from sensor
  // if tape is not centered then
  //    if tape is more to the left then
  //       forward on right motor for ...
  //    else if tape is more to the right then
  //       forward on left motor for ...
  //    end if
  // else if tape is not visible then
  //    reverse on both motors for ...
  // else
  //    forward on both motors for ...
  // end if
  // END Nav Algorithm pseudocode

  uint16_t position = qtr.readLineWhite(sensorValues);
}
