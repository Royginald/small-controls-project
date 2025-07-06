
#ifndef P_VARS
#define P_VARS

#include "Settings.h"

namespace P_Control {
    #ifdef position_feedback
    float K_p = 0.15;
    #endif

    #ifdef speed_feedback
    float K_p = 1;
    #endif

    float control_P(float r, float y);
    void set_P_control_gain(float new_K_p);
}

#endif


