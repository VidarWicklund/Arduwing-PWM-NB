#include <Arduino.h>
#include <Servo.h>
#include <PinChangeInterrupt.h>

#include "pins.h"
#include "variables.h"
#include "calculatedValues.h"
#include "ISRs.h"

// Define the servo objects
Servo esc;
Servo leftFlap;
Servo rightFlap;

void setup() {
  // Debug serial
  Serial.begin(9600);
  
  // Attach the servos and ESC to the appropriate pins
  esc.attach(escPin);
  leftFlap.attach(leftFlapPin);
  rightFlap.attach(rightFlapPin);

  // Set pin modes
  pinMode(throttlePinIn, INPUT);
  pinMode(aileronPinIn, INPUT);
  pinMode(elevonPinIn, INPUT);

  // Attach pin change interrupts
  attachPCINT(digitalPinToPCINT(throttlePinIn), throttleISR, CHANGE);
  attachPCINT(digitalPinToPCINT(aileronPinIn), aileronISR, CHANGE);
  attachPCINT(digitalPinToPCINT(elevonPinIn), elevonISR, CHANGE);

}

void loop() {  
  // Calculate fraction of throttle input as compared to max input
  float throttleAmountInFromMin = (throttleValue - minThrottleIn) / rangeThrottleIn;

  // Calculate fraction of aileron and elevon input as compared to mid (this is to be able to reverse direction easier)
  float aileronAmountInFromMid = (aileronValue - midAileronIn) / (rangeAileronIn / 2);
  float elevonAmountInFromMid = (elevonValue - midElevonIn) / (rangeElevonIn / 2);

  // ESC signal is proportional passthrough of input, no mixing
  int throttleValueToWrite = constrain(minEsc + (rangeEsc * throttleAmountInFromMin), minEsc, maxEsc);

  // Elevon mixing logic
  // Weights for ailerong and elevon are calculated with input and preset mix
  float aileronWeight = aileronAmountInFromMid * aileronMixFrac;
  float elevonWeight = elevonAmountInFromMid * elevonMixFrac;

  // The value starts at minimal value and is raised towards the maximum
  int leftFlapValue = centerLeftFlap + (directionLeftFlap * halfRangeLeftFlap * (elevonWeight + aileronWeight));
  int rightFlapValue = centerRightFlap + (directionRightFlap * halfRangeRightFlap * (elevonWeight - aileronWeight));

  // Output PWM signals to the ESC and servos
  esc.writeMicroseconds(throttleValueToWrite);          // Send throttle to ESC
  leftFlap.writeMicroseconds(constrain(leftFlapValue, minLeftFlap, maxLeftFlap)); // Control left elevon
  rightFlap.writeMicroseconds(constrain(rightFlapValue, minRightFlap, maxRightFlap)); // Control right elevon

  // Add a small delay to ensure smooth servo movement
  delay(smoothingDelay);
}
