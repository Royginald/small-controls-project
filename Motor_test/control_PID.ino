
#include "control_PID.h"

float control_PID(float r, float y) {
    float error = r - y;

    PID_Control::error_sum += error;

    float ctl_act = PID_Control::K_p * error + PID_Control::K_i * PID_Control::error_sum + PID_Control::K_d * (error - PID_Control::prev_error);

    PID_Control::prev_error = error;

    return ctl_act ;
}

void set_PID_control_gains(float new_K_p, float new_K_i, float new_K_d) {
    PID_Control::K_p = new_K_p;
    PID_Control::K_i = new_K_i;
    PID_Control::K_d = new_K_d;
}