title	    "EMK310 Practical 1"
PROCESSOR   18F45K22 

;CONFIG1H
CONFIG FOSC = INTIO67
CONFIG WDTEN = OFF
   
#include    <xc.inc>
#include    "pic18f45k22.inc"
    
#include    "constants.s"
#include    "setup.s"
#include    "interrupts.s"
#include    "calibration.s"
#include    "timing.s"
#include    "line_navigation.s"
    
    
    
PSECT code,abs	; Start Code section
org	0h	; startup address = 0000h
goto setup
goto main
org 08h
