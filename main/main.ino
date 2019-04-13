// Copyright (c) 2019 Maine Township High School West Autonomous
// Vehicle Competiton Team, Sebastian Moruszewicz, Ghifari Aditya,
// Danica Roque, Veronica Kleinschmidt, Daniel Jeong, and Samuel
// "Fish" Hooson. All rights reserved.

#include <QTRSensors.h>
#include <Wire.h>

#define NUM_SENSORS              5        // number of sensors used
#define NUM_SAMPLES_PER_SENSOR   4        // average 4 analog samples per sensor reading
#define EMITTER_PIN              2        // emitter is controlled by digital pin 2

#define MOTOR_SPEED 240            // initial motor speed to un-stall
#define MOTOR_SPEED_COMP_LEFT 15   // motor characteristic speed compensation
#define MOTOR_PIN_LEFT_SPEED A4    // pin to set left motor speed
#define MOTOR_PIN_LEFT_FORWARD A0  // pin to drive left motor forward
#define MOTOR_PIN_LEFT_REVERSE A1  // pin to drive left motor backward
#define MOTOR_PIN_RIGHT_SPEED A5   // pin to set right motor speed
#define MOTOR_PIN_RIGHT_FORWARD A2 // pin to drive right motor forward
#define MOTOR_PIN_RIGHT_REVERSE A3 // pin to drive right motor backward

#define VISIBLE_THRESHOLD -1000    // sensor value threshold for tape visibility
#define LEFT_THRESHOLD    -500     // sensor value threshold for left-leaning
#define RIGHT_THRESHOLD    500     // sensor value threshold for right-leaning

#define LOST_ITERATION_THRESHOLD 10  // recovery algorithm phase 0 iteration threshold

static const uint8_t motor_pins[] = {A0, A1, A2, A3, A4, A5}; //Motor driver interfaced via analog pins

QTRSensors qtr;
uint16_t sensorValues[NUM_SENSORS];

uint16_t lostIteration;
uint16_t recoveryPhase;

void setup()
{
  //Clearing state of motor driver
  for (int i = 0; i < 6; i++)
  {
    pinMode(motor_pins[i], OUTPUT);
    digitalWrite(motor_pins[i], LOW);
  }

  //Setting motor speed
  analogWrite(MOTOR_PIN_LEFT_SPEED, MOTOR_SPEED - MOTOR_SPEED_COMP_LEFT);
  analogWrite(MOTOR_PIN_RIGHT_SPEED, MOTOR_SPEED);

  qtr.setTypeRC();
  // Sensors 0 through 5 are connected to digital pins 2 through 6, respectively
  qtr.setSensorPins((const uint8_t[]){3, 4, 5, 6, 7}, NUM_SENSORS);
  qtr.setEmitterPin(EMITTER_PIN);

  delay(500);
  // TODO: use NeoPixel to indicate calibration phase

  for(uint16_t i = 0; i < 400; i++)
    qtr.calibrate();

  // TODO: use NeoPixel to indicate end of calibration, then turn off

  lostIteration = 0;
  recoveryPhase = 0;
}

//Reset state of BOTH motors
void motor_state_reset()
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(motor_pins[i], LOW);
  }
}

// Forward on BOTH motors
void motor_both_forward()
{
  motor_state_reset();
  digitalWrite(MOTOR_PIN_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_PIN_LEFT_FORWARD, HIGH);
}

// Reverse on BOTH motors
void motor_both_reverse()
{
  motor_state_reset();
  digitalWrite(MOTOR_PIN_RIGHT_REVERSE, HIGH);
  digitalWrite(MOTOR_PIN_LEFT_REVERSE, HIGH);
}

// Forward on LEFT motor only
void motor_left_forward()
{
  motor_state_reset();
  digitalWrite(MOTOR_PIN_LEFT_FORWARD, HIGH);
}

// Forward on RIGHT motor only
void motor_right_forward()
{
  motor_state_reset();
  digitalWrite(MOTOR_PIN_RIGHT_FORWARD, HIGH);
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
