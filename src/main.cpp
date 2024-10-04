#include <Arduino.h>
#include <Servo.h>
#include <PinChangeInterrupt.h>

#include "pins.h"
#include "variables.h"

// Define the servo objects
Servo esc;
Servo leftFlap;
Servo rightFlap;

// Calculates channels midpoints, assumed to mean neutral
const int midThrottleIn = (minThrottleIn + maxThrottleIn)/2;
const int rangeThrottleIn = (maxThrottleIn - minThrottleIn);

const int midAileronIn = (minAileronIn + maxAileronIn)/2;
const int rangeAileronIn = (maxAileronIn - minAileronIn);

const int midElevonIn = (minElevonIn + maxElevonIn)/2;
const int rangeElevonIn = (maxElevonIn - minElevonIn);

// Calculates center and range
const int rangeEsc = maxEsc - minEsc;

const int centerLeftFlap = (minLeftFlap + maxLeftFlap)/2;
const int rangeLeftFlap = (maxLeftFlap - minLeftFlap);
const int halfRangeLeftFlap = rangeLeftFlap / 2;

const int centerRightFlap = (minRightFlap + maxRightFlap)/2;
const int rangeRightFlap = (maxRightFlap - minRightFlap);
const int halfRangeRightFlap = rangeRightFlap / 2;

// Calculates mixing weights
const float aileronMixFrac = aileronMixPercentage / 100;
const float elevonMixFrac = 1 - aileronMixFrac;

// Variables to store pulse widths
volatile unsigned long throttleStart = 0;
volatile float throttleValue = minEsc;  // Default to idle
volatile unsigned long aileronStart = 0;
volatile float aileronValue = midAileronIn;   // Default to center input value
volatile unsigned long elevonStart = 0;
volatile float elevonValue = midElevonIn;  // Default to center input value


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
