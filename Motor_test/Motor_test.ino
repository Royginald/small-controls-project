
// #include <encoder.h>

#define MOTOR_1_PIN 5
#define MOTOR_2_PIN 6

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

#define MOTOR_MAX_VOTLAGE 5
#define K_P 0.1

volatile long encoderPosition = 0;


int setpoint = 500;
int error, control_action;
unsigned long start_time;
int data = 0;

void setup() {
    delay(500);
    pinMode(MOTOR_1_PIN, OUTPUT);
    pinMode(MOTOR_2_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), handleEncoderA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN), handleEncoderB, CHANGE);

    Serial.begin(9600);
    start_time = millis();
    delay(3000);
}

void loop() {
    if (Serial.available() > 0) {
        setpoint = Serial.read();
    }

    Serial.println(getEncoderPosition());

    control_action = control_P(setpoint, getEncoderPosition());

    // error = setpoint - getEncoderPosition();

    // control_action = K_P * error;

    if (control_action > MOTOR_MAX_VOTLAGE) {
        control_action = MOTOR_MAX_VOTLAGE * sgn(control_action);
    }

    if (control_action > 0) {
        analogWrite(MOTOR_1_PIN, control_action/MOTOR_MAX_VOTLAGE*255);
        analogWrite(MOTOR_2_PIN, 0);
    } else {
        analogWrite(MOTOR_1_PIN, 0);
        analogWrite(MOTOR_2_PIN, control_action/MOTOR_MAX_VOTLAGE*255);
    }

    
}

float control_P(float r, float y) {
    return K_P * (r - y);
}

long getEncoderPosition() {
    noInterrupts();
    long pos = encoderPosition;
    interrupts();
    return pos;
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
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
