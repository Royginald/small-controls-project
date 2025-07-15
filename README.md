This repository documents the motor plants that were made for the use of the Inteligent Controls and Injetion moldling lab an UNB.

This repository conains:
 - The CAD files needed to 3D print the brackets for the motor (cad)
 - The wiring digram used to wire the Arduino R4 to the motor and motor driver (wiring_setup)
 - An Arduino program to control the motor from an Ardino R4 (Motor_Control)
 - Some sample responses (in steps/s) of the motor at various voltages (Motor Responses)
 - A system identification program that can be run from the Ardunio R4 (Sys_ID)
 - A response visualizer that can dispaly the motors response on a PC (serial.comm.py)


For all control algorithums used, the motor is treated as a first order system with the following parameters:

Plant gain: 501.16 steps/s per volt
Plant time constant: 0.16046 seconds

For speed control (Where speed is measured in steps/s):

$T(s) = \frac{501.16}{0.16046 s + 1}$

For position control (Where position is measured in steps):

$T(s) = \frac{501.16}{s(0.16046 s + 1)}$

The encoder has 1320 steps per revolution.
