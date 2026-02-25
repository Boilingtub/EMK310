title	    "EMK310 Practical 1"
PROCESSOR   18F45K22 

;CONFIG1H
CONFIG FOSC = INTIO67
CONFIG WDTEN = OFF
   
#include    <xc.inc>
#include    "pic18f45k22.inc"
    
#include    "constants.inc"
#include    "shared_macros.inc"
        
PSECT code,abs	; Start Code section
org	0h	; startup address = 0000h
goto main
org 08h ;High priority Interrupt Vector
goto ISR
org 18h	;Low priority Interrupt Vector
goto ISR
org 20h	;Start for code setup
 
#include "setup.inc"
#include "timer.inc"
#include "interrupts.inc"
#include "calibration.inc"
#include "line_location_interpreter.inc"
 

    
main:   
   call setup
   call RGB_calibrate_test
   bra exit
   
exit:
    bra $-2
    
ISR:
    retfie
    




