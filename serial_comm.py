# -*- coding: utf-8 -*-
"""
Created on Tue Mar 25 13:31:57 2025

@author: rmcintos
"""

import serial
import numpy as np
import time
import csv
import matplotlib.pyplot as plt

def setpoint(t):
    return 100 * np.sin(3*t) + 900

runtime = 5 # Seconds
port_number = 4
file_name = "motor_data.csv"

# --------------- Main Program  --------------- 

plt.close()
data = []

try:
    s = serial.Serial('COM' + str(port_number), 9600)
    
    print("Starting control")
    inst_time = 0
    start_time = time.time()
    
    while(inst_time < runtime):
        inst_time = time.time() - start_time
        sp = float(setpoint(inst_time))
        res = s.readline()
        
        if res == b'\r\n':
            continue
        elif b'\x00' in res:
            continue
        else:    
            res = float(res)
        
        data.append([inst_time, sp, res])
        # print(res)
        b = bytearray()
        b.extend(map(ord, "{:.2f}".format(sp)))
        s.write(b)
        s.reset_input_buffer()
        s.reset_output_buffer()
     
    s.reset_output_buffer()
    for i in range(12):
        s.write(b'0.00\r\n')
        
    print("Data collection complete")
    s.reset_input_buffer()
    s.reset_output_buffer()
    s.dtr = False
    s.close()
    
    # --------------- Plot Data  --------------- 
    
    time  = [d[0] for d in data]
    setpt = [d[1] for d in data]
    resp  = [d[2] for d in data]
    # res_test = [d[3] for d in data]
    
    plt.close()
    plt.plot(time, setpt, label="Setpoint")
    plt.plot(time, resp, label="Response")
    plt.grid()
    plt.legend()
    
    # --------------- Save Data  --------------- 
    
    print("Saving Data")
    with open(file_name, "w", newline="\n") as file:
        writer = csv.writer(file, delimiter=",")
        writer.writerow(["Time", "Setpoint", "Position"])
        
        for row in data:
            writer.writerow(row)
            
except serial.SerialException:        
    print("Serial Port not Avalible")
except Exception as e:
    print("Error Occured: Closing")
    print(e)
    
    s.reset_input_buffer()
    s.reset_output_buffer()
    s.dtr = False
    s.close()

