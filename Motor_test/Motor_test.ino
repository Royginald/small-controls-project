
#include "Motor_test.h"

float setpoint = 0;
int error, control_action;

volatile long encoderPosition = 0;
long prev_encoder_pos = 0;
float speed = 0;

unsigned long loop_start_time, prog_start_time;
long dt = 0;

void setup() {
    delay(500);

    pinMode(MOTOR_1_PIN, OUTPUT);
    pinMode(MOTOR_2_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), handleEncoderA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), handleEncoderB, CHANGE);

    Serial.setTimeout(1); //Milliseconds
    Serial.begin(9600);
    
    prog_start_time = micros();
    loop_start_time = micros();
}

void loop() {
    // if (Serial.available() > 0) {
    //     setpoint = Serial.readString().toFloat();
    // }

    setpoint = step(float((micros() - loop_start_time)/1000), 2500.0);

    dt = (micros() - loop_start_time);

    if (dt > (TIME_STEP * 1000000)) {
        loop_start_time = micros();
        speed = (getEncoderPosition() - prev_encoder_pos)*1000000.0/dt;
        prev_encoder_pos = getEncoderPosition();

        long send_pos = getEncoderPosition() % STEPS_PER_REVOLUTION; 
        if (send_pos < 0)
            send_pos += STEPS_PER_REVOLUTION;

        Serial.println(int(float(send_pos) * 360. / STEPS_PER_REVOLUTION));
        // Serial.println(speed);
        // Serial.println(control_action);

        // control_action = control_P(setpoint, getEncoderPosition());
        control_action = control_PI(setpoint, getEncoderPosition());
        // control_action = control_PID(setpoint, getEncoderPosition());
        // control_action = setpoint;

        if (abs(control_action) > MOTOR_MAX_VOLTAGE) {
            control_action = MOTOR_MAX_VOLTAGE * sgn(control_action);
        }

        if (control_action > 0) {
            analogWrite(MOTOR_1_PIN, control_action*255/MOTOR_MAX_VOLTAGE);
            analogWrite(MOTOR_2_PIN, 0);
        } else {
            analogWrite(MOTOR_1_PIN, 0);
            analogWrite(MOTOR_2_PIN, -control_action*255/MOTOR_MAX_VOLTAGE);
        }
    }
}

long getEncoderPosition() {
    noInterrupts();
    long pos = encoderPosition;
    interrupts();
    return pos;
}

void addEncoderPosition(long add) {
    noInterrupts();
    encoderPosition += add;
    interrupts();
    // return pos;
}

void handleEncoderA() {
    if (digitalRead(ENCODER_A_PIN) == digitalRead(ENCODER_B_PIN)) {
        encoderPosition++;
    } else {
        encoderPosition--;
    }
}

void handleEncoderB() {
    if (digitalRead(ENCODER_A_PIN) != digitalRead(ENCODER_B_PIN)) {
        encoderPosition++;
    } else {
        encoderPosition--;
    }
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
