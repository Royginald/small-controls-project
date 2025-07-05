
#ifndef SETPOINT_VARS
#define SETPOINT_VARS

namespace setpoints {
    float step(float time, double value);
    float step(float time, float value);
    float step(float time, long value);
    float step(float time, int value);
    
    float ramp(float time, float slope);
    float sine(float time, float frequency, float aplitude);
}

#endif
