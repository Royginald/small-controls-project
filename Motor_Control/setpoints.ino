
// --------------------------< Setpoints >--------------------------
// 
// This file contains a varity of different setpoint profiles

#include "setpoints.h"

float step(float time, double value) {
    return float(value);
}

float step(float time, float value) {
    return float(value);
}

float step(float time, long value) {
    return float(value);
}

float step(float time, int value) {
    return float(value);
}

float ramp(float time, float slope) {
    return time * slope;
}

float sine(float time, float frequency, float amplitude) {
    return amplitude * sin(frequency * time);
}
