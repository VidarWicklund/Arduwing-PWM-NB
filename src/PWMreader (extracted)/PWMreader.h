//Simplified code for PWM reads, see PWMread.me for explanation

#include <Arduino.h>
#include <PinChangeInterrupt.h>

const int pinToDetect = 1; // Pin GPIO number
volatile unsigned long pulseStarted; // Variable to store the "timestamp" for when the pin went high
volatile int pulseWidth;

const int pulseMin = 1000; // Sets shortest accepted pulse
const int pulseMax = 2000; // Sets longest accepted pulse

void ISRFunction() {                      // Triggered by a change on PWM pin
  if (digitalRead(pinToDetect) == HIGH) { // Detects that the pin went to high, that means the pulse started at this timestamp
    pulseStarted = micros();               // Store timestamp for start of pulse

  } else {                                // Detects that the pin went to low, that means the pulse ended at this timestamp
    pulseWidth = micros() - pulseStarted;  // Takes current timestamp and subtracts when the pulse started, giving the pulse width
    pulseWidth = constrain(pulseWidth, pulseMin, pulseMax); // This is just to make sure the pulse isn't set to something outside the expected
  }
}

void setup() {
attachPCINT(digitalPinToPCINT(pinToDetect), ISRFunction, CHANGE); // Tells controller to do hardware interrupts
}

void loop() {

}