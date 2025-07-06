


#ifndef PI_VARS
#define PI_VARS

#include "Settings.h"

namespace PI_Control {
    #ifdef position_feedback
    float K_p = 0.8;
    float K_i = 0.5;
    #endif

    #ifdef speed_feedback
    float K_p = 1;
    float K_i = 1;
    #endif

    float error_sum = 0;
    float control_PI(float r, float y);
    void set_PI_control_gains(float new_K_p, float new_K_i);
}

#endif