title	    "EMK310 Practical 1"
PROCESSOR   18F45K22 

;CONFIG1H
CONFIG FOSC = INTIO67
CONFIG WDTEN = OFF
   
#include    <xc.inc>
#include    "pic18f45k22.inc"
    
#include    "constants.inc"
        
PSECT code,abs	; Start Code section
org	0h	; startup address = 0000h
goto setup
goto main
org 08h ;Interrupt vector
goto ISR
 
#include "setup.s"
#include "interrupts.s"
#include "calibration.s"
#include "timer.s"
#include "line_location_interpreter.s"
 
main:
   call calibrate_all
   goto exit
   
ISR:
    
    return
    
exit:
end
