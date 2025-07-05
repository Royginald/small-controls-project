#ifndef CONTROL_MPC_H
#define CONTROL_MPC_H

#include "Motor_Control.h"

namespace MPC_Control {
    // properties used in the MPC loop:
    const int N = 20;                        // prediction window
    const int nu = 1;                        // control move window
    const float minU = -MOTOR_MAX_VOLTAGE;   // min voltage threshold 
    const float maxU =  MOTOR_MAX_VOLTAGE;   // max voltage threshold
    const float lambda = 0;                  // move suppression lambda, not implemented.

    // dynamic matrix:
    const float p[N] = {156.124735202492, 263.612481245330, 337.614948147096, 388.563687042082, 423.640544661371,
                                        447.790032305804, 464.416315076581, 475.863070504438, 483.743858509286, 489.169572369321, 
                                        492.905032690405, 495.476798207413, 497.247390666162, 498.466396689165, 499.305650057026, 
                                        499.883453777579, 500.281256339081, 500.555132868962, 500.743689607603, 500.873505929222};

    // manipulated dynamic matrix:
    const float ppp[N] = {0.0000377, 0.0000637, 0.0000815, 0.0000939, 0.0001023,
                            0.0001082, 0.0001122, 0.0001149, 0.0001168, 0.0001182,
                            0.0001191, 0.0001197, 0.0001201, 0.0001204, 0.0001206, 
                            0.0001207, 0.0001208, 0.0001209, 0.0001210, 0.0001210};

    // state variables:
    float y_hat[N] = {0};   // predictions of the state
    float u_prev = 0;       // previous control action
    float E[N] = {0};       // error
    float dU = 0;           // change in control action

    // functions:
    float control_MPC(float setpoint, float y_meas);

}   // MPC_Control

#endif // CONTROL_MPC_H