
// --------------------------< Settings >--------------------------
//
// This file is used to select the settings that will be used for motor system
// This includes the control algorithum, setpoint type, and type of feedback
// Uncomment the option you wish to use for each option

// -------- Control Algorithum -------- 

// #define deadbeat_control
// #define MPC_control
// #define PID_control
#define PI_control
// #define P_control
// #define Fixed_voltage

// Note1: MPC control only works with speed feedback
// Note2: PID with speed control needs some fine tuning

// -------- Setpoint Type -------- 
// Setpoint settings can be changed on line 78 of Motor_control.ino

// #define step_setpoint
#define ramp_setpoint
// #define sine_setpoint

// -------- FeedBack Type -------- 

// #define speed_feedback
#define position_feedback




