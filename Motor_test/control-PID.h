
#ifndef PID_VARS
#define PID_VARS

namespace PID_Control {
    float K_p = 1;
    float K_i = 1;
    float K_d = 1;
 
    float error_sum = 0;
    float prev_error = 0;
    float control_PID(float r, float y);
    void set_PID_control_gains(float new_K_p, float new_K_i, float new_K_d);
}
#endif

