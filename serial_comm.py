# -*- coding: utf-8 -*-
"""
Created on Tue Mar 25 13:31:57 2025

NOTE: Press CTRL + C on the IDE to end the loop while not in fixed time mode.

@author: rmcintos
"""

import serial
import numpy as np
import time
import csv
import matplotlib.pyplot as plt

fixed_time = False
runtime = 5 # Seconds
time_step = 0.02 # Seconds

# def setpoint(t):
#     return 100 * np.sin(3*t) + 900

def setpoint(t):
    return 12


port_number = 4
file_name = "motor_data.csv"

data = np.zeros((int(runtime/time_step), 3))

# --------------- Main Program  --------------- 

plt.close()

try:
    s = serial.Serial('COM' + str(port_number), 9600)
    print("Starting control")
    inst_time = 0
    start_time_loop = time.time() - 2*time_step
    start_time = time.time()
    
    if fixed_time:
        i = 0
        while(i < int(runtime/time_step)):
            if (time.time() - start_time_loop > time_step):
                
                # Update time
                start_time_loop = time.time()
                inst_time = time.time() - start_time
                
                # Read data
                s.reset_input_buffer()
                res = s.readline()
                
                
                if res == b'\r\n':
                    continue
                elif b'\x00' in res:
                    continue
                elif float(res) == 0 in res:
                    continue
                elif res == b'-\n':
                    res = s.readline()
                    res = -1 * float(res)
                else:    
                    res = float(res)
                
                
                # data.append([inst_time, sp, res])
                # print(res)
                
                # Send Setpoint
                sp = 0
                # sp = float(setpoint(inst_time))
                # b = bytearray()
                # b.extend(map(ord, "{:.2f}".format(sp)))
                # s.write(b)
                
                # s.reset_output_buffer()
                
                # Update data
                print("Data Logged")
                print(res)
                print(inst_time)
                # print(i)
                data[i, :] = [inst_time, sp, res]
                i += 1
                
        print("Data collection complete")
        s.reset_input_buffer()
        s.reset_output_buffer()
        s.dtr = False
        s.close()
        
        # --------------- Plot Data  --------------- 
        
        time  = [d[0] for d in data]
        setpt = [d[1] for d in data]
        resp  = [d[2] for d in data]
        
        # speed = [(data[i][2] - data[i-1][2]) / data[i][0] for i in range(2, len(data))]
        speed = np.gradient(resp, time_step, edge_order=2)
        # res_test = [d[3] for d in data]
        
        plt.close()
        # plt.plot(time, setpt, label="Setpoint")
        # plt.plot(time, speed, label="Speed")
        plt.plot(time, resp, label="Response")
        plt.xlabel("Time (s)")
        # plt.ylabel("Speed (steps/s)")
        plt.ylabel("Position (steps)")
        plt.grid()
        plt.legend()
        
        # --------------- Save Data  --------------- 
        
        print("Saving Data")
        with open(file_name, "w", newline="\n") as file:
            writer = csv.writer(file, delimiter=",")
            writer.writerow(["Time (s)", "Voltage (V)", "Speed (steps/s)"])
            
            for row in data:
                writer.writerow(row)
                
    else:
        plt.close()
        
        plt.ion()
        
        fig = plt.figure()
        ax = fig.add_subplot()
        
        line = ax.plot(data[0, :], data[2, :])[0]
        ax.grid()
        ax.legend()
        
        try:
            while(True):
                if (time.time() - start_time_loop > time_step):
                    
                    # Update time
                    start_time_loop = time.time()
                    inst_time = time.time() - start_time
                    
                    # Read data
                    s.reset_input_buffer()
                    res = s.readline()
                    
                    
                    if res == b'\r\n':
                        continue
                    elif b'\x00' in res:
                        continue
                    elif float(res) == 0 in res:
                        continue
                    elif res == b'-\n':
                        res = s.readline()
                        res = -1 * float(res)
                    else:    
                        res = float(res)
                        
                    sp = 0
                      
                    # Update data
                    print("Time: %.2f" % inst_time)
                    data[:-1, :] = data[1:, :]
                    data[-1, :] = [inst_time, sp, res]
                    
                    line.set_data(data[:, 0], data[:, 2])
                    ax.set(xlim=[data[0, 0], data[-1, 0] ])
                    ax.set(ylim=[0, 360])
                    plt.draw()
                    plt.pause(0.01)
                    
        except KeyboardInterrupt:
            print("Loop Ended")
                    
        s.reset_input_buffer()
        s.reset_output_buffer()
        s.dtr = False
        s.close() 
            
except serial.SerialException:        
    print("Serial Port not Avalible")
except Exception as e:
    print("Error Occured: Closing")
    print(e)
    
    s.reset_input_buffer()
    s.reset_output_buffer()
    s.dtr = False
    s.close()
    
    
    
    
    

