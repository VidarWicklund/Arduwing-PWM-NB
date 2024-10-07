#ifndef CALCULATEDVALUES_H
#define CALCULATEDVALUES_H

#include <variables.h>

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


#endif
