// Copyright (c) 2019 Maine Township High School West Autonomous
// Vehicle Competiton Team, Sebastian Moruszewicz, Ghifari Aditya,
// Danica Roque, Veronica Kleinschmidt, Daniel Jeong, and Samuel
// "Fish" Hooson. All rights reserved.

#include <QTRSensors.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define NUM_SENSORS              6        // number of sensors used
#define NUM_SAMPLES_PER_SENSOR   4        // average 4 analog samples per sensor reading
#define EMITTER_PIN              2        // emitter is controlled by digital pin 2

#define MOTOR_DURATION           1        // duration of each motor "step" (in secs)
#define MOTOR_SPEED              150      // speed of each motor
#define MOTOR_PIN_LEFT           1        // pin location of the left motor
#define MOTOR_PIN_RIGHT          3        // pin location of the right motor

#define VISIBLE_THRESHOLD        -1000    // sensor value threshold for tape visibility
#define LEFT_THRESHOLD           -500     // sensor value threshold for left-leaning
#define RIGHT_THRESHOLD          500      // sensor value threshold for right-leaning

#define LOST_ITERATION_THRESHOLD 10       // recovery algorithm phase 0 iteration threshold

QTRSensors qtr;
uint16_t sensorValues[NUM_SENSORS];

Adafruit_MotorShield shield;
Adafruit_DCMotor *motorLeft;
Adafruit_DCMotor *motorRight;

uint16_t lostIteration;
uint16_t recoveryPhase;

void setup()
{
  qtr.setTypeAnalog();
  // sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
  qtr.setSensorPins((const uint8_t[]){0, 1, 2, 3, 4, 5}, NUM_SENSORS);
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

  lostIteration = 0;
  recoveryPhase = 0;
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

void recovery_alg_reset()
{
  recoveryPhase = 0;
  lostIteration = 0;
}

void loop()
{
  // BEGIN Nav Algorithm pseudocode
  // read from sensor
  // if tape is not visble then
  //   if recovery phase is not 1 and iteration is less than threshold then
  //     reverse on both motors
  //     increase iteration
  //   else if recovery phase is not 1 and iteration is greater than threshold then
  //     set recovery phase to 1
  //     forward on both motors
  //     decrease iteration
  //   else
  //     forward on left motor
  //   end
  // else if tape is left-leaning then
  //   set recovery phase to 0
  //   set iteration to 0
  //   forward on left motor
  // else if tape is right-leaning then
  //   set recovery phase to 0
  //   set iteration to 0
  //   forward on right motor
  // else
  //   set recovery phase to 0
  //   set iteration to 0
  //   forward on both motors
  // end
  // END Nav Algorithm pseudocode

  uint16_t position = qtr.readLineWhite(sensorValues);
  int16_t error = position - 1000;

  if(position < VISIBLE_THRESHOLD)
  {
    if(recoveryPhase != 1 && lostIteration < LOST_ITERATION_THRESHOLD)
    {
      // TODO: use NeoPixel to indicate start of recovery phase 0
      motor_both_reverse();
      lostIteration++;
    }
    else if(recoveryPhase != 1 && lostIteration > 0)
    {
      recoveryPhase = 1;
      motor_both_forward();
      lostIteration--;
    }
    else
    {
      // TODO: use NeoPixel to indicate start of recovery phase 1
      motor_left_forward();
    }
  }
  else if(error < LEFT_THRESHOLD)
  {
    // TODO: use NeoPixel to indicate tape is left-leaning
    recovery_alg_reset();
    
    motor_left_forward();
  }
  else if(error > RIGHT_THRESHOLD)
  {
    // TODO: use NeoPixel to indicate tape is right-leaning
    recovery_alg_reset();
    
    motor_right_forward();
  }
  else
  {
    // TODO: use NeoPixel to indicate normal operation
    recovery_alg_reset();

    motor_both_forward();
  }
}
