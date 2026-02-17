title	    "Program 1 Physical"
PROCESSOR   18F45K22 

;CONFIG1H
CONFIG FOSC = INTIO67
;CONFIG WDTEN = OFF
    
#include    <xc.inc>
#include    "pic18f45k22.inc"

Delay1 EQU 0x2000
Delay2 EQU 0x2001
next_state EQU 0x2002
    
PSECT code,abs	; Start Code section
org	0h	; startup address = 0000h

;Initialize Port A (check Data Sheet)
movlb	0xF	    ;Set BSR for banked SFRs
clrf	PORTA, a	    ; Init port A, clear output of data latches
clrf	LATA, a	    ; Alternate way to clear all data latches
clrf	ANSELA, b    ; Make Digital Inputs
clrf	TRISA, a	    ; SET RA<3:0> Input and RA<4:7> Output
goto	on

on:
    movlw   0xFF		; move 00001111b to W register
    movwf   PORTA, a		; move W to port A     
    movlw   0			; move 1 to W register
    movwf   next_state,a	; move W register to next_state
    goto    delay
    
off:
    movlw   0x00 		; move 00000000b to W register
    movwf   PORTA, a		; move W to portA   
    movlw   1			; move 1 to W register
    movwf   next_state,a	; move W to next_state (0x2002)
    goto    delay 			
   
delay:
    movlw   0xFF	;move 0xFF to W
    movwf   Delay1, a	;move W to Delay1 (0x2000)
    movwf   Delay2, a	;move W to Delay2 (0x2001)
delay_l:
    decfsz  Delay1, a   ;decrement value at Delay1 (0x2000) if 0 skip next line
    goto    delay_l	;continue loop
    
    movwf   Delay1, a	;move W to Delay1 [reset Delay 1 value to 0xFF]
    decfsz  Delay2, a   ;decrement value at Delay2 (0x2001) if 0 skip next line
    goto    delay_l	;continue loop
        
    TSTFSZ  next_state, a ;if next_state is 0 skip next line
    goto    on
    goto    off