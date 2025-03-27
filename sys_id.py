# -*- coding: utf-8 -*-
"""
Created on Thu Mar 27 09:45:37 2025

@author: rmcintos

Used to read a bunch of step responses that have been measured from a motor and
to find the first order approximation of that system.
"""

import numpy as np
import csv
from scipy.interpolate import interp1d
from sklearn.metrics import r2_score

steady_state_percent = 0.7
min_voltage = 3
max_voltage = 12
num_files = max_voltage - min_voltage + 1
voltages = np.linspace(min_voltage, max_voltage, num_files)

max_speeds = np.zeros((num_files))
rise_times = np.zeros((num_files))

for i in range(min_voltage, max_voltage + 1):
    file_name = "Motor Responses\motor_data_" + str(i) +"_volts.csv"
    data = []
    
    with open(file_name, "r", newline="\n") as file:
        reader = csv.reader(file, delimiter=",")
        reader.__next__() # get rid of header
        
        for row in reader:
            data.append([float(item) for item in row])
            
        max_speeds[i-min_voltage] = np.average([d[2] for d in data[int(len(data) * (1 - steady_state_percent)):]])
        rise_speed = max_speeds[i-min_voltage] * 0.63
        
        rise_times[i-min_voltage] = interp1d([d[2] for d in data], [d[0] for d in data])(rise_speed)
        
p   = np.polyfit(voltages, max_speeds, 1)
tau = np.average(rise_times)
R2  = r2_score(max_speeds, voltages*p[0] + p[1])

print("Plant gain: " + str(p[0]) + " steps/s per volt")
print("Plant time constant: " + str(tau) + " seconds")



