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
goto setup
 
goto main
org 08h ;Interrupt vector
goto ISR

#include "timer.inc"
#include "setup.inc"
#include "interrupts.inc"
 
#include "calibration.inc"
#include "line_location_interpreter.inc"
 
lli_init:
    MOVLW 0xFF
    MOVWF Sensor 
 
main:
    press:
	BTFSC PORTB,4
	bra press
 
    release:
	BTFSS PORTB,4
	bra release
	
    call input_stepper;function to run through LUT
    MOVFF Sensor,LATD;outputs the current stage of lut to led's
   ;goto exit
   
input_stepper:;to run trough lut table but will have to reset regester here
    INCF Sensor, f
    ;BCf LATA,4;clears the led output, but only for signle led
    MOVF Sensor,w
    XORLW 0x20;to test when the sensor hits 0b00100000 reset to 0
	BZ reset_var
    CLRF LATA;clear the latch to make led's go off 
    return
   
reset_var:
    MOVLW 0x00
    MOVWF Sensor
    return
   
ISR:
    
    return
    
exit:
end






