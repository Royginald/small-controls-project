Plant gain: 501.1603764220277 steps/s per volt
Plant time constant: 0.16046421877501083 seconds


# Setpoint Specification
## Speed setpoint 
The setpoint sent to the motor is specified in encoder count per second (CPS). Given a quadrature encoder with 11 pulse and a 30:1 gear reduction ratio between the motor and the output shaft, the conversion rate between encoder count and the output shaft is calculated as

output_shaft_RPM = encoder_CPS / (reduction_ratio * encoder_pulses * 4 for quadrature) * 60 seconds/minute
output_shaft_RPM = encoder_CPS / (30 * 11 * 4) * 60

For example, if you want the ouput shaft to spin at 120 RPM, the CPS setpoint should be 2640 