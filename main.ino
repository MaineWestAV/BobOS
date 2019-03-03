#include <QTRSensors.h>
#include <Arduino.h>
#include <wiring.h>

#define NUM_SENSORS 6            // number of sensors used
#define NUM_SAMPLES_PER_SENSOR 4 // average 4 analog samples per sensor reading
#define EMITTER_PIN 2            // emitter is controlled by digital pin 2

// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog qtra((unsigned char[]){0, 1, 2, 3, 4, 5},
                      NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

void setup()
{
}

void loop()
{
}
