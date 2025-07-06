
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
// Note2: Gains for P and PI control have only been calculated for position feedback
// Note3: No gains for PID control have been calculated

// -------- Setpoint Type -------- 

// #define step_setpoint
#define ramp_setpoint
// #define sine_setpoint

// -------- FeedBack Type -------- 

// #define speed_feedback
#define position_feedback




