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
#include "calibration_white.inc"
#include "line_location_interpreter.inc"
 
main: 
    clrf rcalib,a
    clrf LATE,a
   
    ;call calibrate
    nop
    call calc_ranges_test
    ;call detect_color_test
    set_main_lli:
	btfsc PORTB,0,a
	bra $+6
	movlw 0
	bra do_main_lli
   
	btfsc PORTB,1,a
	bra $+6
	movlw 1
	bra do_main_lli
   
	btfsc PORTB,2,a
	bra $+6
	movlw 2
	bra do_main_lli
   
	btfsc PORTB,3,a
	bra $+6
	movlw 3
	bra do_main_lli
   
    do_main_lli:
	movwf nav_col,a
	call Sensor_LLI_Generate
	bra set_main_lli
	
   bra exit
   
exit:
    nop
    bra $-2