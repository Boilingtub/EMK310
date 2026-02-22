title	    "Program 1 Physical"
PROCESSOR   18F45K22 

;CONFIG1H
CONFIG FOSC = INTIO67
;CONFIG WDTEN = OFF
    
#include    <xc.inc>
#include    "pic18f45k22.inc"

Delay1 EQU 0x1
Delay2 EQU 0x2
flash_counter EQU 0x3
rgb_color EQU 0x4
tmp EQU	0x5
PORTB_val EQU 0x6

RED_val	    EQU 0b00000001
GREEN_val   EQU 0b00000010
BLUE_val    EQU 0b00000100
WAIT_FACTOR EQU	200

toggle_RGB macro out_port, addr_port, rgb_val
    movff   addr_port,  WREG, a ;Move Port B into Working register 
    andlw   rgb_val
    tstfsz  WREG
    bra	    $+4
    bra	    $+6
    movlw   0b00000000
    bra	    $+4
    movlw   rgb_val
    movwf   addr_port,a
    movwf   out_port,a
endm

take_RGB_measurement macro in_port, rgb_port_val
    
endm
    
wait_n_cycles macro num, l_addr1, l_addr2
    movlw   num
    movwf   l_addr1, a
    movwf   l_addr2, a
    decfsz  l_addr1, a 
    bra     $-2
    movwf   l_addr1,a 
    decfsz  l_addr2, a
    bra     $-8
endm

calibrate_RGB macro count_addr, count_val, out_port, addr_port, rgb_val
    movlw count_val
    movwf count_addr
    toggle_RGB out_port, addr_port, rgb_val   ;toggle_RGB is 22 bytes
    take_RGB_measurement PORTC, 
    wait_n_cycles WAIT_FACTOR, Delay1, Delay2 ;wait_n_cycles is 16 bytes
    decfsz  count_addr,f		      ;decfsz is 2 bytes
    bra	    $-40			      ;put PC 40 bytes back
endm

 
PSECT code,abs	; Start Code section
org	0h	; startup address = 0000h


;Initialize Port A (check Data Sheet)
movlb	0xF	    ;Set BSR for banked SFRs
clrf	PORTA, a	    ; Init port A, clear output of data latches
clrf	LATA, a	    ; Alternate way to clear all data latches
clrf	ANSELA, b    ; Make Digital Inputs
clrf	TRISA, a	    ; SET all RA digital Out
 
;Initialize Port B (check Data Sheet)
movlb	0xF	;Set BSR for Banked SFR
clrf	PORTB,a	;clear output of data latches
clrf	LATB, a	;alternative method
clrf	ANSELB,a;Enable RB<3:0> for digital Inputs
clrf    TRISB,a	;Set all RB digital OUT
 
;Initialize Port C (check Data Sheet)
movlb	0xF	;Set BSR for Banked SFR
clrf	PORTB,a	;clear output of data latches
clrf	LATB, a	;alternative method
clrf	ANSELB,a;Enable RB<3:0> for digital Inputs
clrf    TRISB,a	;Set all RB digital OUT
 
start:
    call RGB_calibrate_all
    
RGB_calibrate_all:
    calibrate_RGB flash_counter, 6, PORTB, PORTB_val, RED_val
    calibrate_RGB flash_counter, 6, PORTB, PORTB_val, GREEN_val
    calibrate_RGB flash_counter, 6, PORTB, PORTB_val, BLUE_val
    return
    