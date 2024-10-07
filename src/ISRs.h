#ifndef ISRS_H
#define ISRS_H

#include <Arduino.h>
#include "pins.h"
#include "variables.h"
#include "calculatedValues.h"

// Interrupt Service Routine (ISR) for throttle (should return 1000-2000)
void throttleISR() {
  if (digitalRead(throttlePinIn) == HIGH) {
    throttleStart = micros();  // Rising edge: start timing
  } else {
    throttleValue = constrain(micros() - throttleStart, minThrottleIn, maxThrottleIn);  // Falling edge: calculate pulse width
  }
  if (throttleValue <= safetyEsc) { //Safety for throttle
    throttleValue = minEsc;
  }
}

// ISR for aileron (should return 1000-2000)
void aileronISR() {
  if (digitalRead(aileronPinIn) == HIGH) {
    aileronStart = micros();
  } else {
    aileronValue = constrain(micros() - aileronStart, minAileronIn, maxAileronIn);
  }
}

// ISR for elevon (should return 1000-2000)
void elevonISR() {
  if (digitalRead(elevonPinIn) == HIGH) {
    elevonStart = micros();
  } else {
    elevonValue = constrain(micros() - elevonStart, minElevonIn, maxElevonIn);
  }
}

#endif