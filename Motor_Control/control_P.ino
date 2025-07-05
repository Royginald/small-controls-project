
// --------------------------< Proportional Control >--------------------------

#include "control_P.h"

float control_P(float r, float y) {
    return P_Control::K_p * (r - y);
}

void set_P_control_gain(float new_K_p) {
    P_Control::K_p = new_K_p;
}