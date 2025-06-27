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
    // RESET FOR EACH PASS:
    dU = 0;

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
        dU += ppp[i] * E[i];
    }
    dU /= (1 + lambda);

    // 4) compute and constrain control action:
    float u = u_prev + dU;
    u = constrain(u, minU, maxU);

    // 5) update predictions with control move:
    // this incorporates the effect of the control move into the prediction
    for(int i = 0; i < N; i++){
        y_hat[i] += p[i]*dU;
    }

    // 6) advance control horizon:
    for(int i = 0; i < N-1; i++){
        y_hat[i] = y_hat[i+1];
    }
    y_hat[N-1] = y_hat[N-2];

    // 7) store previous control action:
    u_prev = u;

    // DEBUGGING:
    // Serial.print("Meas: "); Serial.print(y_meas);
    // Serial.print(" dU: "); Serial.print(dU);
    // Serial.print(" U: "); Serial.println(u);

    // return active control action:
    return u;
}