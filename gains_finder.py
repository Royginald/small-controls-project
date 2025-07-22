# -*- coding: utf-8 -*-
"""
Created on Wed Jul 16 09:03:39 2025

@author: rmcintos
"""

P_CONTROL   = 0
PI_CONTROL  = 1
PID_CONTROL = 2

SPEED_FEEDBACK    = 0
POSITION_FEEDBACK = 1

import numpy as np
import control as ct
from scipy.optimize import minimize

# --------------------------- Options --------------------------- 

# Plant settings
tau = 0.16046 # seconds
K_p = 501.16 # steps/s

# Closed Loop Desired Specs
percent_OS = 5.0 # percent
T_s        = 3 # seconds
SS_error   = 5.0 # percent

# 0 -> P Control
# 1 -> PI Control
# 2 -> PID Control
control_type = P_CONTROL

# 0 -> Speed feedback
# 1 -> Position feedback
feedback_type = SPEED_FEEDBACK
# feedback_type = POSITION_FEEDBACK

# --------------------------- Calculations --------------------------- 

K_c, K_i, K_d = (0, 0, 0)

percent_OS /= 100
SS_error   /= 100

def test_gains(t, Kp, Kc, Ki, Kd, tolerance=0.02):
    if feedback_type == SPEED_FEEDBACK:
        plant = ct.tf([Kp], [t, 1])
    elif feedback_type == POSITION_FEEDBACK:
        plant = ct.tf([Kp], [t, 1, 0])
    
    controller = ct.tf([Kd, Kc, Ki], [1, 0])
    
    sys = ct.feedback(controller * plant, 1)
    
    ss = ct.tf2ss(sys)
    
    X_o = [0] * len(ss.B)
    
    time = np.linspace(0, 0.0006, num=7)
    setpoint = [1] * 6
    
    def mini(x):
        (tim, out) = ct.forced_response(sys, X0=x, T=time[:6], U=setpoint)
        
        if len(X_o) > 1:
            slope = (out[5] - out[0]) / ( tim[5] - tim[0])
            return (out[0] - X_o[0])**2 + 10*(slope - X_o[1])**2
        else:
            return (out[0] - X_o[0])**2
    
    res1 = minimize(mini, X_o, method='nelder-mead',
           options={'xatol': 1e-4, 'disp': False})

    X_o_best = res1.x
    
    (T, Y) = ct.step_response(sys, X0=X_o_best)
    
    y_final = Y[-1]
    y_peak = np.max(Y)
    # peak_time = T[np.argmax(y)]
    
    overshoot = ((y_peak - y_final) / y_final) * 100 if y_final != 0 else 0

    # Settling time
    lower = y_final * (1 - tolerance)
    upper = y_final * (1 + tolerance)
    settling_indices = np.where((Y < lower) | (Y > upper))[0]
    if settling_indices.size > 0:
        settling_time = T[settling_indices[-1] + 1] if (settling_indices[-1] + 1) < len(T) else T[-1]
    else:
        settling_time = 0.0
        
    final_error = abs(y_final - 1)
        
    return (overshoot, settling_time, final_error)

def cost_func(os_meas, Ts_meas, os_des, Ts_des, ss_e):
    return abs(os_des - os_meas)/os_des + abs(Ts_des - Ts_meas)/Ts_des + ss_e
            

if feedback_type == SPEED_FEEDBACK:
    if control_type == P_CONTROL: # Good
        K_c = (1/SS_error - 1) / K_p
        
    
    if control_type == PI_CONTROL: # Good
        zeta = -np.log(percent_OS) / np.sqrt( np.pi**2 + np.log(percent_OS)**2)
        
        K_i = 16 * tau / (K_p * zeta **2 * T_s**2)
        K_c = ( 8 * tau / T_s - 1 ) / K_p
        
    
    if control_type == PID_CONTROL: # Good
        def mini(x):
            os_res, T_s_res, f_error = test_gains(tau, K_p, x[0], x[1], x[2])
            return cost_func(os_res/100, T_s_res, percent_OS, T_s, f_error)
        
        X0 = [.001, .01, .004]
        
        res = minimize(mini, X0, method='nelder-mead',
               options={'xatol': 1e-6, 'disp': False})
        
        (K_c, K_i, K_d) = res.x
            
    
if feedback_type == POSITION_FEEDBACK:
    if control_type == P_CONTROL: # Good
        K_c = (1/SS_error) / K_p
    
    if control_type == PI_CONTROL: # Mediocre
        zeta = -np.log(percent_OS) / np.sqrt( np.pi**2 + np.log(percent_OS)**2)
        w_n = 4 / (zeta * T_s)
        
        P3 = 1/tau - 2 * zeta * w_n
        
        if P3 < 0:
            print("ERROR: System cannot reach these CL Specs")
            print("Try increasing the %OS or increasing the settling time")
        else:
            K_c = tau/K_p * (P3 * 2 * zeta * w_n + w_n**2)
            K_i = tau/K_p * (P3 * w_n**2)
            K_d = 0
            
            if P3 / (zeta*w_n) < 10:
                print("WARNING: 3rd pole is cloase to the main CL poles")
                print("Try increasing the %OS or increasing the settling time")
                
    
    if control_type == PID_CONTROL: # Alright
        zeta = -np.log(percent_OS) / np.sqrt( np.pi**2 + np.log(percent_OS)**2)
        w_n = 4 / (zeta * T_s)
        
        P3 = 10 * zeta * w_n
        
        K_d = ( (P3 + 2 * zeta * w_n) * tau - 1 ) / K_p
        K_i = tau * P3 * w_n**2 / K_p
        K_c = tau * ( P3 * 2 * zeta * w_n + w_n**2) / K_p
 
if K_c:
    print("Proportional gain: " + str(K_c))

if K_i:
    print("Integral gain: " + str(K_i))

if K_d:
    print("Derivative gain: " + str(K_d))
        
os_res, T_s_res, f_error = test_gains(tau, K_p, K_c, K_i, K_d)
cost = cost_func(os_res/100, T_s_res, percent_OS, T_s, f_error)

print("------------------------------------------------------------")
print(f"Measured overshoot: {os_res} %")
print(f"Measured settling time: {T_s_res} s")
print(f"Measured steady state error: {f_error*100} %")
print(f"Overall Cost: {cost}")
    
    
    
    