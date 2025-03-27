
#include "control_P.h"
#include "control-PID.h"


#define MOTOR_1_PIN 5
#define MOTOR_2_PIN 6

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

#define MOTOR_MAX_VOLTAGE 12
#define TIME_STEP 0.001

int setpoint = 0;
int error, control_action;

volatile long encoderPosition = 0;
long prev_encoder_pos = 0;
float speed = 0;

unsigned long start_time;
long dt = 0;

void setup() {
    delay(500);

    pinMode(MOTOR_1_PIN, OUTPUT);
    pinMode(MOTOR_2_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), handleEncoderA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), handleEncoderB, CHANGE);

    Serial.setTimeout(1); //Milliseconds
    Serial.begin(9600);
    start_time = micros();

}

void loop() {
    if (Serial.available() > 0) {
        setpoint = Serial.readString().toFloat();
    }

    dt = (micros() - start_time);

    if (dt > (TIME_STEP * 1000000)) {
        start_time = micros();
        speed = (getEncoderPosition() - prev_encoder_pos)*1000000.0/dt;
        prev_encoder_pos = getEncoderPosition();

        // Serial.println(getEncoderPosition());
        Serial.println(speed);
        // Serial.println(control_action);

        // control_action = control_P(setpoint, getEncoderPosition());
        // control_action = control_PID(setpoint, getEncoderPosition());
        control_action = setpoint;


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
