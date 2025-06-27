#include "control_MPC.h"
#include "Motor_test.h"

// array printing function:
// void MPC_Control::print_Array(float arr[], int N) {
//     // for every value in the array:
//     for(int i = 0; i < N; i++){
//         Serial.println(arr[i]);
//     }
// }

// main control function:
float MPC_Control::control_MPC(float setpoint, float y_meas){
    Serial.println(setpoint);
    // 1) adjust predictions:
    float delta = y_meas - y_hat[0];
    for(int i = 0; i < N; i++){
        y_hat[i] += delta;
    }

    // 2) evaluate the error:
    for(int i = 0; i < N; i++){
        E[i] = setpoint - y_hat[i];
    }

    // 3) compute control move:
    for(int i = 0; i < N; i++){
        dU += ppp[0][i] * E[i];
    }

    // 4) compute and constrain control action:
    float u = u_prev + dU;
    if (u > maxU) u = maxU;
    if (u < minU) u = minU;

    // 5) update predictions with control move:
    // this incorporates the effect of the control move into the prediction
    for(int i = 0; i < N; i++){
        y_hat[i] += p[0][i]*dU;
    }

    // 6) advance control horizon:
    for(int i = 0; i < N; i++){
        y_hat[i] = y_hat[i+1];
    }

    y_hat[N-1] = y_hat[N-2];

    // 7) store previous control action:
    u_prev = u;

    // return active control action:
    return u;
}