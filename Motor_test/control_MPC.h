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
    extern const float ppp[nu][N];

    // state variables:
    extern float y_hat[N];
    extern float u_prev;

    // functions:
    float control_MPC(float setpoint, float y_meas);
    void print_Array(float arr[], int N);

}   // MPC_Control

#endif // CONTROL_MPC_H