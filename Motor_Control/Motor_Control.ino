
// --------------------------< Main File >--------------------------
// 
// This is the main file of this program which contains the setup() and loop() fucntions
// Each other file is for a specific type of control algorithum
// The control algorithum used and the setpoint are set in this file

#include "Motor_Control.h"
using namespace MPC_Control;

float setpoint = 0;
float control_action;

volatile long encoderPosition = 0;
long prev_encoder_pos = 0;
float speed = 0;

unsigned long loop_start_time, prog_start_time;
long dt = 0;

void setup() {
    delay(500);

    // Set motor PWM pins as output pins
    pinMode(MOTOR_1_PIN, OUTPUT);
    pinMode(MOTOR_2_PIN, OUTPUT);

    // Set up the encoder pins to handel interupts
    attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), handleEncoderA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), handleEncoderB, CHANGE);

    // Start a serial connection over USB and set the timeout to 1 ms for fast data transfer
    Serial.setTimeout(1); //Milliseconds
    Serial.begin(9600);
    
    // Track the start of the program and the start of the latest loop
    prog_start_time = micros();
    loop_start_time = micros();
}

void loop() {
    // if (Serial.available() > 0) {
    //     setpoint = Serial.readString().toFloat();
    // }

    // Calcualte the value of the setpoint based on the time since the program started
    float program_time = float((micros() - prog_start_time)/1000000); // Runtime in seconds
    setpoint = step(program_time, 1500.0);
    // setpoint = sine(program_time, 0.01, 500.0) + 2000;
    // setpoint = ramp(program_time, 50.0);

    // Find the time since the loop
    dt = float((micros() - loop_start_time) / 1000000 ); // Time since last loop in seconds

    // If the time since the last loop is greater then the time step, calcualte a new cotnrol action
    if (dt > TIME_STEP) {
        // Reset the loop timer
        loop_start_time = micros();

        // Calcualte the current motor speed and position
        speed = (getEncoderPosition() - prev_encoder_pos)*1000000.0/dt;
        prev_encoder_pos = getEncoderPosition();

        // Find the angle of the motor shaft in degrees
        long send_pos = getEncoderPosition() % STEPS_PER_REVOLUTION; 
        if (send_pos < 0)
            send_pos += STEPS_PER_REVOLUTION;
        float motor_angle = float(send_pos) * 360. / STEPS_PER_REVOLUTION;

        // Serial.println(int(float(send_pos) * 360. / STEPS_PER_REVOLUTION));
        // Serial.println(getEncoderPosition());
        // Serial.println(speed);

        // Based on the user selected control algorithum, the control action will be calculated
        // control_action = control_P(setpoint, getEncoderPosition());
        // control_action = control_PI(setpoint, getEncoderPosition());
        control_action = control_PID(setpoint, getEncoderPosition());
        // control_action = control_MPC(setpoint, getEncoderPosition());
        // control_action = setpoint;
        // control_action = MOTOR_MAX_VOLTAGE;

        // The process variable is printed out via the serial connection
        // Serial.println(control_action);
        // Serial.println(setpoint);
        Serial.println(getEncoderPosition());
        // Serial.print("Meas: "); Serial.print(speed);
        // Serial.print(",Setpoint: "); Serial.println(setpoint);
        // Serial.println(speed);

        // Limit the control action if it is above the maximum voltage of the motor driver
        if (abs(control_action) > MOTOR_MAX_VOLTAGE) {
            control_action = MOTOR_MAX_VOLTAGE * sgn(control_action);
        }

        // Send the control action to the motor driver while accounting for its sign
        if (control_action > 0) {
            analogWrite(MOTOR_1_PIN, control_action*255/MOTOR_MAX_VOLTAGE);
            analogWrite(MOTOR_2_PIN, 0);
        } else {
            analogWrite(MOTOR_1_PIN, 0);
            analogWrite(MOTOR_2_PIN, -control_action*255/MOTOR_MAX_VOLTAGE);
        }
    }
}

// Function to get the position of the encoder
long getEncoderPosition() {
    noInterrupts();
    long pos = encoderPosition;
    interrupts();
    return pos;
}

// Function to add or subtract some nubmer of steps from the encoder position
void addEncoderPosition(long add) {
    noInterrupts();
    encoderPosition += add;
    interrupts();
    // return pos;
}

// Interupt service routine for the first pin of the encoder
void handleEncoderA() {
    if (digitalRead(ENCODER_A_PIN) == digitalRead(ENCODER_B_PIN)) {
        encoderPosition++;
    } else {
        encoderPosition--;
    }
}

// Interupt service routine for the second pin of the encoder
void handleEncoderB() {
    if (digitalRead(ENCODER_A_PIN) != digitalRead(ENCODER_B_PIN)) {
        encoderPosition++;
    } else {
        encoderPosition--;
    }
}

// Template for an absolute value fucntion
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


