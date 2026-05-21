title	    "EMK310 Practical 2"
PROCESSOR   18F45K22 

;CONFIG1H
CONFIG FOSC = INTIO67
CONFIG WDTEN = OFF
CONFIG CCP2MX = PORTB3
CONFIG CCP3MX = PORTE0
   
#include    <xc.inc>
#include    "pic18f45k22.inc"
    
#include    "constants.inc"
        
PSECT code,abs	; Start Code section
org	0h	; startup address = 0000h
call setup
goto main
org 08h ;Interrupt Vector
retfie
 
#include "race.inc"
#include "setup.inc"
#include "calibration.inc"
#include "touch.inc"
#include "nav_col.inc"
 
main:
    call calibrate_start
    call Set_Nav_col
    call wait_for_touch
    call Race
    bra $-4
    
exit:
    bra $