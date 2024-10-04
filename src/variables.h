#ifndef VARIABLES_H
#define VARIABLES_H

// Define PWM input limits (microseconds), same limits as the receiver sends
const int minThrottleIn = 1100;
const int maxThrottleIn = 1770;
const int safetyEsc = 1200;

const int minAileronIn = 1088;
const int maxAileronIn = 1820;

const int minElevonIn = 1212;
const int maxElevonIn =1850;

// Define ESC limits and safety point (under which there is no movement) (microseconds)
const int minEsc = 1000;
const int maxEsc = 2000;

// Define servo limits (microseconds)
const int minLeftFlap = 1200;
const int maxLeftFlap = 1900;

const int minRightFlap = 1000;
const int maxRightFlap = 1700;

// Reverse servos (set to 1 or -1)
const int directionRightFlap = 1;
const int directionLeftFlap = -1;

// Define mixing aileron/elevon in percent aileron (calculations are done in main.cpp)
const float aileronMixPercentage = 40;

// Define smoothing delay between loops
const int smoothingDelay = 0;

#endif