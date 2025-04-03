
#ifndef PI_VARS
#define PI_VARS

namespace PI_Control {
    float K_p = 0.8;
    float K_i = 0.0005;
 
    float error_sum = 0;
    float control_PI(float r, float y);
    void set_PI_control_gains(float new_K_p, float new_K_i);
}

#endif