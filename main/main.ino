// Copyright (c) 2019 Maine Township High School West Autonomous
// Vehicle Competiton Team, Sebastian Moruszewicz, GHIFARI160 and
// associates, Danica Roque, Veronica Kleinschmidt, Daniel Jeong,
// and Samuel "Fish" Hooson. All rights reserved.
//
// Autonomous Vehicle Competition Team
// Team Lead: Sebastian Moruszewicz
// Secretary: Danica Roque
// Presenter: Samuel "Fish" Hooson
// Members:   Ghifari Aditya, Daniel Jeong, Veronica Kleinschmidt
//
// OID
// ASN.1:   {iso(1) identified-organization(3) dod(6) internet(1) private(4) enterprise(1) 53559 avc(5) rover(1) bob(1)}
// dot:     1.3.6.1.4.1.53559.5.1.1
// OID-IRI: /ISO/Identified-Organization/6/1/4/1/53559/5/1/1

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
