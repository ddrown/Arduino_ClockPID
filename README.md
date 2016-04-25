ClockPID calculates the rate adjustment the local clock needs based on offset
measurements.  It uses a PID controller to do the calculation.  The D term in
PID is calculated through a Theil-Sen estimator.

functions:

float add\_sample(int32\_t timestamp, int32\_t raw\_offset, int32\_t corrected\_offset)
 -- timestamp - the time the sample was taken, in milliseconds
 -- raw\_offset - the raw (uncorrected) offset between the local and remote clock
 -- corrected\_offset - the corrected offset between the local and remote clock

float p()
float p\_out()
 -- the last P term (in seconds) and factor (in ppm) used in the PID controller

float i()
float i\_out()
 -- the last I term (in seconds) and factor (in ppm) used in the PID controller

float d()
float d\_out()
 -- the last D term (in seconds) and factor (in ppm) used in the PID controller

float d\_chi()
 -- the chi-squared "Goodness of fit" for the D term's estimation

float out()
 -- the final factor (in ppm)
