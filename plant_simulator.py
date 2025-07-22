# -*- coding: utf-8 -*-
"""
Created on Wed Jul 16 10:58:04 2025

@author: rmcintos
"""

STEP_INPUT = 0
RAMP_INPUT = 1

import control as ct
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import minimize

tau = 0.16046 # seconds
K_p = 501.16 # steps/s

setpoint = 2000
X_o = [0, 0]

K_c = 0.0012828366898020196
K_i = 0.004834878121977795
K_d = 0.0008351180335681176

add_feedback = True
position_control = False

sim_time = 7
time_step = 0.001


# --------------------------- Functions --------------------------- 

def step_info(t, y, tolerance=0.02):
    y_final = y[-1]
    y_peak = np.max(y)
    peak_time = t[np.argmax(y)]
    
    overshoot = ((y_peak - y_final) / y_final) * 100 if y_final != 0 else 0

    # Settling time
    lower = y_final * (1 - tolerance)
    upper = y_final * (1 + tolerance)
    settling_indices = np.where((y < lower) | (y > upper))[0]
    if settling_indices.size > 0:
        settling_time = t[settling_indices[-1] + 1] if (settling_indices[-1] + 1) < len(t) else t[-1]
    else:
        settling_time = 0.0

    return {
        "Overshoot (%)": overshoot,
        "Peak Time (s)": peak_time,
        "Settling Time (s)": settling_time,
        "Final Value": y_final,
    }

# --------------------------- Simulating System --------------------------- 

def mini(x):
    (tim, out) = ct.forced_response(sys, X0=x, T=time[:6], U=setpoint)
    
    slope = (out[5] - out[0]) / ( tim[5] - tim[0])
    return (out[0] - X_o[0])**2 + 10*(slope - X_o[1])**2
    

if position_control:
    plant = ct.tf([K_p], [tau, 1, 0])
    units = "steps"
else:
    plant = ct.tf([K_p], [tau, 1])
    units = "steps/s"
    
controller = ct.tf([K_d, K_c, K_i], [1, 0])

if add_feedback:
    sys = ct.feedback(controller * plant, 1)
else:
    sys = plant

time = np.arange(0, sim_time, time_step)

res = minimize(mini, X_o, method='nelder-mead',
       options={'xatol': 1e-9, 'disp': False})

X_o_best = res.x

(time, outputs) = ct.forced_response(sys, X0=X_o_best, T=time, U=setpoint)


info = step_info(time, outputs)

ss_error = setpoint - info["Final Value"]

width = 8
precision = 4

print(f"Final Value        : {info['Final Value']:{width}.{precision}} " + units)
print(f"Steady state error : {ss_error:{width}.{precision}} " + units)
print(f"Peak time          : {info['Peak Time (s)']:{width}.{precision}} seconds" )
print(f"Percent overshoot  : {info['Overshoot (%)']:{width}.{precision}} %")
print(f"Settling Time      : {info['Settling Time (s)']:{width}.{precision}} seconds")


plt.plot([time[0], time[-1]], [info['Final Value']]*2, ":", label = "Final Value")
plt.plot([time[0], time[-1]], [setpoint]*2, "--", label = "Setpoint")
plt.plot(time, outputs, label="Response")
plt.xlabel("Time (s)")
plt.ylabel("Response (" + units + ")")
plt.grid()
plt.legend()

# print(outputs[0])



