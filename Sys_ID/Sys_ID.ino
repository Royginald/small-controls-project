
// #include "Motor_test.h"
#define TEST_VOLTAGE 6
#define TEST_TIME 5
#define NUM_DATA_POINTS 1000

#define MOTOR_1_PIN 5
#define MOTOR_2_PIN 6

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

#define MOTOR_MAX_VOLTAGE 12
#define TIME_STEP 0.001

// float setpoint = 0;
// int error, control_action;

volatile long encoderPosition = 0;
// long prev_encoder_pos = 0;
// float speed = 0;

unsigned long loop_start_time, prog_start_time;
long dt = TEST_TIME / NUM_DATA_POINTS;
long data[NUM_DATA_POINTS];

int i = 0;

void setup() {
    delay(500);

    pinMode(MOTOR_1_PIN, OUTPUT);
    pinMode(MOTOR_2_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), handleEncoderA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), handleEncoderB, CHANGE);

    Serial.setTimeout(1); //Milliseconds
    Serial.begin(9600);
    
    // prog_start_time = micros();
    loop_start_time = micros();
}

void loop() {
    // if (Serial.available() > 0) {
    //     setpoint = Serial.readString().toFloat();
    // }

    // Collect data
    analogWrite(MOTOR_1_PIN, TEST_VOLTAGE*255/MOTOR_MAX_VOLTAGE);

    while(i < NUM_DATA_POINTS) {
        if (dt * 1000000 < (micros() - loop_start_time)) {
            loop_start_time = micros();
            data[i] = getEncoderPosition();
            i++;
        }
    }

    analogWrite(MOTOR_1_PIN, 0);

    // Process data
    // Integrate
    // find end value
    // find time of 63%
    // have tau and k_p


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
