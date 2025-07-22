
#ifndef PID_VARS
#define PID_VARS

#include "Settings.h"

namespace PID_Control {
    #ifdef position_feedback
    float K_p = 0.012579263379904046;
    float K_i = 0.012579263379904046;
    float K_d = 0.003127464282863755;
    #endif

    #ifdef speed_feedback
    float K_p = 0.0012828366898020196;
    float K_i = 0.004834878121977795;
    float K_d = 0.0008351180335681176;
    #endif

    float error_sum = 0;
    float prev_error = 0;
    float control_PID(float r, float y);
    void set_PID_control_gains(float new_K_p, float new_K_i, float new_K_d);
}

#endif

