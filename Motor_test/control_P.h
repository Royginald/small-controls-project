
#ifndef P_VARS
#define P_VARS

namespace P_Control {
    float K_p = 0.15;
    float control_P(float r, float y);
    void set_P_control_gain(float new_K_p);
}

#endif


