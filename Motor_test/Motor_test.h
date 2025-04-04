#include "control_P.h"
#include "control_PI.h"
#include "control_PID.h"
#include "setpoints.h"

#define MOTOR_1_PIN 5
#define MOTOR_2_PIN 6

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

#define MOTOR_MAX_VOLTAGE 12
#define TIME_STEP 0.001