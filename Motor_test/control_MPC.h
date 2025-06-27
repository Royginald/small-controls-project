#ifndef CONTROL_MPC_H
#define CONTROL_MPC_H

namespace MPC_Control {
    // properties used in the MPC loop:
    const int N = 20;           // prediction window
    const int nu = 1;           // control move window
    const float minU = 0.0f;    // min voltage threshold 
    const float maxU = 9.0f;    // max voltage threshold
    const float lambda = 1.1;   // move suppression lambda

    // manipulated dynamic matrix:
    extern const float ppp[nu][N] = {0.0000377, 0.0000637, 0.0000815, 0.0000939, 0.0001023,
                        0.0001082, 0.0001122, 0.0001149, 0.0001168, 0.0001182,
                        0.0001191, 0.0001197, 0.0001201, 0.0001204, 0.0001206, 
                        0.0001207, 0.0001208, 0.0001209, 0.0001210, 0.0001210};

    // state variables:
    extern float y_hat[N][1] = 0.0;
    extern float u_prev = 0.0;

    // functions:
    float control_MPC(float setpoint, float y_meas);
    void init_MPC();

}   // MPC_Control

#endif // CONTROL_MPC_H