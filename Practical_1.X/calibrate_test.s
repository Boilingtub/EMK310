title	    "EMK310 Practical 1"
PROCESSOR   18F45K22 

;CONFIG1H
CONFIG FOSC = INTIO67
CONFIG WDTEN = OFF
CONFIG CCP3MX = PORTE0
   
#include    <xc.inc>
#include    "pic18f45k22.inc"
    
#include    "constants.inc"
#include    "shared_macros.inc"
        
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
#include "timer.inc"
#include "interrupts.inc"
#include "calibration_PWM_impl.inc"
#include "line_location_interpreter.inc"
 
main: 
   flash_Reg flash_counter, 3, DUMP_REG, 0b11111111
   call pwm_setup
   call calibrate
   call calibrate_test_cont
   bra exit
   
exit:
    nop
    bra $-2