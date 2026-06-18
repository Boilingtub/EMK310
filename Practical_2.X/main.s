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
org 08h ;High priority Interrupt Vector
goto ISRH
 
org 18h	;Low priority Interrupt Vector
goto ISRL
 
org 20h	;Start for code setup 
#include "setup.inc"
#include "pwm_setup.inc"
#include "timer.inc"
#include "Sensor.inc"
#include "touch.inc"
#include "color_detection.inc"
#include "interrupts.inc"
#include "calibration.inc"
#include "line_location_interpreter.inc"
 
main:
		     ;2_F ;1_F   ;2_B ;1_B
    ;set_motor_pwm   0x00,0xFA,  0x00,0x00
    ;call calibrate_test
    call calibrate_start
    call Check_Nav_Col
    call start_on_touch
    call Detect_LLI
    bra $-4
    bra exit
   
exit:
    nop
    bra $-2


