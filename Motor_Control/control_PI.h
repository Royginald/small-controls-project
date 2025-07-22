


#ifndef PI_VARS
#define PI_VARS

#include "Settings.h"

namespace PI_Control {
    #ifdef position_feedback
    float K_p = 0.04239358625600699;
    float K_i = 0.004261333802572665;
    #endif

    #ifdef speed_feedback
    float K_p = 0.0005660467714901427;
    float K_i = 0.010756669964191914;
    #endif

    float error_sum = 0;
    float control_PI(float r, float y);
    void set_PI_control_gains(float new_K_p, float new_K_i);
}

#endif