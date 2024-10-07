#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "pins.h"
#include "variables.h"
#include "calculatedValues.h"

void debug(){
  Serial.print("Throttle value/amount/out: ");
  Serial.print(throttleValue);
  Serial.print("/");
  Serial.print(throttleAmountInFromMin);
  Serial.print("/");
  Serial.print(throttleValueToWrite);
  Serial.print("\t");

  Serial.print("Aileron in: ");
  Serial.print(aileronValue);
  Serial.print("/");
  Serial.print(aileronWeight);
  Serial.print("\t");

  Serial.print("Elevon in: ");
  Serial.print(elevonValue);
  Serial.print("/");
  Serial.print(elevonWeight);
  Serial.print("\t");

  Serial.print("Left value: ");
  Serial.print(constrain(leftFlapValue, minLeftFlap, maxLeftFlap));
  Serial.print("\t");

  Serial.print("Right value: ");
  Serial.print(constrain(rightFlapValue, minRightFlap, maxRightFlap));
  Serial.print("\n"); 
}

#endif