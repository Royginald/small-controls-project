
// #include "Motor_test.h"
#define TEST_VOLTAGE 6
#define TEST_TIME 5.
#define NUM_DATA_POINTS 1500
#define END_TOL 0.9

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
double dt = TEST_TIME / NUM_DATA_POINTS;
long data[NUM_DATA_POINTS]; // Steps
float data_diff[NUM_DATA_POINTS - 1]; // Steps per second
float tau;

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
    Serial.println("Starting Test");
    // Serial.println(String(dt));
    analogWrite(MOTOR_1_PIN, TEST_VOLTAGE*255/MOTOR_MAX_VOLTAGE);

    i = 0;

    while(i < NUM_DATA_POINTS) {
        if (dt * 1000000 < (micros() - loop_start_time)) {
            loop_start_time = micros();
            data[i] = getEncoderPosition();
            i++;
        }
    }

    analogWrite(MOTOR_1_PIN, 0);
    Serial.println("Test Finished");
    Serial.println("Starting Processing");

    // Process data
    // Differentate

    for (int j = 0; j < NUM_DATA_POINTS - 1; j++) {
        data_diff[j] = ( data[j+1] - data[j] ) / dt;
    }

    float end_value = float(data_diff[NUM_DATA_POINTS-2]) * END_TOL;

    // Serial.println(data_diff[NUM_DATA_POINTS-2]);
    // Serial.println(END_TOL);
    // Serial.println(end_value);
    // Serial.println(" ------------------ ");

    i = NUM_DATA_POINTS-3;
    while(data_diff[i] > end_value && i >= 0){
        // Serial.println(data_diff[i]);
        i--;
    }

    // Serial.println(i);

    float end_time = i * dt / END_TOL;
    int end_index = i / END_TOL;

    float average = 0;

    for (int j = end_index; j < NUM_DATA_POINTS-2; j++) {
        average += data_diff[j];
    }

    average /= NUM_DATA_POINTS - 1;

    float K_p = average / TEST_VOLTAGE;

    float rise_value = average * 0.63;

    i = end_index;
    while(data_diff[i] > rise_value && i >= 0) {
        i--;
    }

    // float tau = (dt) / (data_diff[i+1] - data_diff[i]) * (rise_value - data_diff[i]) + i * dt;
    tau = i * dt;

    Serial.println("Plant gain: " + String(K_p) + " steps/s / V");
    Serial.print("Plant rise time: ");
    Serial.print(tau, 6);
    Serial.println(" seconds");
    // Serial.println(end_index);
    // Serial.println(i);

    // for (int j = 0; j < NUM_DATA_POINTS-1; j++) {
    //     Serial.print(j * dt);
    //     Serial.print(" ");
    //     Serial.println(data_diff[j]);
    // }

    delay(TEST_TIME * 1000.);

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
