
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

    // Find the time since the loop
    dt = float((micros() - loop_start_time)); // Time since last loop in microseconds

    // If the time since the last loop is greater then the time step, calculate a new cotnrol action
    if (dt > TIME_STEP * 1000000) {
        // Reset the loop timer
        loop_start_time = micros();

        // Calculate the current motor speed
        speed = (getEncoderPosition() - prev_encoder_pos)*1000000.0/dt; // Speed of motor, in steps per second
        prev_encoder_pos = getEncoderPosition(); // Position of the motor, in steps

        // Find the angle of the motor shaft in degrees
        long send_pos = getEncoderPosition() % STEPS_PER_REVOLUTION; 
        if (send_pos < 0)
            send_pos += STEPS_PER_REVOLUTION;
        float motor_angle = float(send_pos) * 360. / STEPS_PER_REVOLUTION;

        // Find the feedback based on the feedback type selected
        #ifdef speed_feedback
        float feedback = speed;
        Serial.println(speed);
        #endif

        #ifdef position_feedback
        float feedback = getEncoderPosition();
        Serial.println(getEncoderPosition());
        #endif

        // Calcualte the value of the setpoint based on the time since the program started
        float program_time = float((micros() - prog_start_time)/1000000.); // Runtime in seconds

        #ifdef step_setpoint
        setpoint = step(program_time, 2500.0);
        #endif

        #ifdef ramp_setpoint
        setpoint = ramp(program_time, 80.0);
        #endif

        #ifdef sine_setpoint
        setpoint = sine(program_time, 0.01, 500.0) + 2000;
        #endif  

        // Based on the user selected control algorithum, the control action will be calculated
        // The units of the control action are volts
        #ifdef deadbeat_control
        #endif

        #ifdef MPC_control
        control_action = control_MPC(setpoint, feedback);
        #endif
        
        #ifdef PID_control
        control_action = control_PID(setpoint, feedback);
        #endif

        #ifdef PI_control
        control_action = control_PI(setpoint, feedback);
        #endif

        #ifdef P_control
        control_action = control_P(setpoint, feedback);
        #endif

        #ifdef Fixed_voltage
        control_action = 3;
        #endif   

        // The process variable is printed out via the serial connection
        // Serial.println(control_action);
        // Serial.println(program_time);
        // Serial.println(getEncoderPosition());
        // Serial.print("Meas: "); Serial.print(speed);
        // Serial.print(",Setpoint: "); Serial.println(setpoint);
        // Serial.println(speed);

        // Serial.println(int(float(send_pos) * 360. / STEPS_PER_REVOLUTION));
        // Serial.println(getEncoderPosition());
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


