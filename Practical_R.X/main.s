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
    ;call Set_Nav_col 
    movlw 2
    movwf nav_col
    
    movlw SSD_1 
    movwf PORTA
    call wait_for_touch
    call set_SSD_from_nav_col
    call Race
    bra $-4
    
RGB_Test:
    movlw 0b01011001 ;AN22 a.k.a RD2, ADC on
    movwf ADCON0,0
    setf PORTC
    bcf PORTC,0
    nop
    ADC_measure 0x1
    
    setf PORTC
    bcf PORTC,1
    nop
    ADC_measure 0x2

    setf PORTC
    bcf PORTC,2
    nop
    ADC_measure 0x3
  
    bra RGB_Test



    
    return