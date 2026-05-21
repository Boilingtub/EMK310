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
 
#include "setup.inc"
#include "pwm_setup.inc"
#include "timer.inc"
#include "Sensor.inc"
#include "touch.inc"
#include "color_detection.inc"
#include "interrupts.inc"
#include "calibration.inc"
#include "line_location_interpreter.inc"
#include "eeprom.inc"
#include "serial.inc"
#include "modes.inc"
#include "simulate.inc"
 
main:
    ;bra prac_2_loop
    goto p3_main
    ;bra pwr_debug
    ;call read_touch
    ;bra $-4
    ;call tx_startup_message
    ;call eeprom_test
    ;goto main
    
exit:
    bra $
    
p3_main: 
    call EEPROM_default
    movlw SSD_8
    movwf PORTA
  
    bsf PORTB,0
    
    call EEPROM_startup_message
    lfsr 0,i2c_sto_addr
    call tx_FSR0
    movlw SSD_7
    
    movwf PORTA
    clrf rflags
    
    movlw 0
    movwf nav_col
    movlw 'J'
    movwf cyoc
    movlw 'A'
    movwf mode_reg
    p3_loop:
	;btfss PORTB,1
	;btfss PORTB,2
    
	movlw 'C'
	cpfseq mode_reg
	bra $+4
	goto p3_color_select
	
	movlw 'R'
	cpfseq mode_reg
	bra $+4
	goto p3_calibrate
	
	movlw 'A'
	cpfseq mode_reg
	bra $+4
	goto p3_attack
	
	movlw 'S'
	cpfseq mode_reg
	bra $+4
	goto p3_simulate
	
	movlw 'H'
	cpfseq mode_reg
	bra $+4
	goto p3_hotload
	
	movf cyoc,0,0
	
	cpfseq mode_reg
	bra $+4
	goto p3_cyoc
	
prac_2_loop:
    		    ;2_F ;1_F   ;2_B ;1_B
    ;set_motor_pwm   0x00,0xFA,  0x00,0x00
    call calibrate_start
    call Check_Nav_Col
    call start_on_touch
    call Detect_LLI
    bra $-4
    bra exit
    
    
pwr_debug:
    clrf PORTA
    bsf PORTA,0
    pwr_debug_loop:
	rlcf PORTA
	wait_timer H333ms,L333ms
	bra pwr_debug_loop
    


