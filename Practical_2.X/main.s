title	    "EMK310 Practical 1"
PROCESSOR   18F45K22 

;CONFIG1H
CONFIG FOSC = INTIO67
CONFIG WDTEN = OFF
CONFIG LVP = ON
   
#include    <xc.inc>
#include    "pic18f45k22.inc"
    
#include    "constants.inc"
#include    "shared_macros.inc"
        
PSECT code,abs	; Start Code section
org	0h	; startup address = 0000h
goto setup
goto main
org 08h ;Interrupt vector
goto ISR
 
#include "setup.inc"
#include "interrupts.inc"
#include "calibration.inc"
#include "timer.inc"
#include "line_location_interpreter.inc"
 
main:
   call RGB_calibrate_test
   goto exit
   
ISR:
    
    return
    
exit:
end
