
#include "control_PI.h"
#include "Motor_test.h"

float control_PI(float r, float y) {
    float error = r - y;

    PI_Control::error_sum += error * TIME_STEP;

    float ctl_act = PI_Control::K_p * error + PI_Control::K_i * PI_Control::error_sum;

    return ctl_act;
}

void set_PI_control_gains(float new_K_p, float new_K_i, float new_K_d) {
    PI_Control::K_p = new_K_p;
    PI_Control::K_i = new_K_i;
}