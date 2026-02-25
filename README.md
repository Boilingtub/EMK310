# Port Layout
## Port A:
### RA<0:1>
RA<0:1> will be used for the calibration status output

## Port B:
### RB<3:0>
RB<3:0> will be used for other generic inputs that do NOT require the use of an interrupt on change

RB<0> will be used for the ADC input
### RB<7:4>
these 4 pins have the special interrupt on change function and will be used to interrupt inputs
RB<4> will be used for the Register Dump input

## Port C:
### RC<2:0>
RC<2:0> used to control RGB Leds

## Port D:
### RD<7:0> Register Dump 
will be used for the register dump

## Port E:
