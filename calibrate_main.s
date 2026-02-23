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
PORTB_val EQU 0x5
ADC_RH	EQU 0x6
ADC_RL EQU 0x7
reg_R EQU 0x8
reg_G EQU 0x9
reg_B EQU 0xB
reg_K EQU 0xD
reg_W EQU 0xE

RGB_R	EQU 0b00000001
RGB_G   EQU 0b00000010
RGB_B   EQU 0b00000100
RGB_W	EQU 0b00000111
WAIT_FACTOR EQU	1


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

take_RGB_measurement macro reg_col
    movlw 0b00000001 ;AN0, ADC on
    movwf ADCON0,a
    bsf	  ADCON0,1,1 ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, ADC_RH 	; Result is complete - store 2 MSbits in
    movff ADRESL, ADC_RL	; RESULTHI and 8 LSbits in RESULTLO
    movff ADRESL, reg_col
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

calibrate_RGB macro reg_col, count_addr, count_val, out_port, addr_port, rgb_val
    movlw count_val
    movwf count_addr
    toggle_RGB out_port, addr_port, rgb_val   ;toggle_RGB is	22 bytes
    ;take_RGB_measurement reg_col	      ;Measure is	22 bytes
    call ADC_measure
    wait_n_cycles WAIT_FACTOR, Delay1, Delay2 ;wait_n_cycles is 16 bytes
    decfsz  count_addr,f		      ;decfsz is	2 bytes
    bra	    $-44			      ;put PC		62 bytes back
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
movlw	0b00001000
movwf	ANSELB, a ; RB<0:2> OUT, RB<3> IN
movlw	0b00001000
movwf   TRISB,	a   ;Set RB<0:2,4:7> to Digital, RB<3> to Analog


;Initialize ADC (Check DataSheet)
movlw 0b00101111 ;left justify, Frc, 12 TAD ACQ time
movwf ADCON2,a
movlw 0b00000000 ;ADC ref = Vdd,Vss
movwf ADCON1,a
 
start:
    call RGB_calibrate_test
    goto exit
   
RGB_calibrate_all:
    calibrate_RGB reg_R, flash_counter, 6, PORTB, PORTB_val, RGB_R
    calibrate_RGB reg_G, flash_counter, 6, PORTB, PORTB_val, RGB_G
    calibrate_RGB reg_B, flash_counter, 6, PORTB, PORTB_val, RGB_B
    calibrate_RGB reg_W, flash_counter, 6, PORTB, PORTB_val, RGB_W
    return
    
RGB_calibrate_test:
    movlw   RGB_W
    movwf   PORTB,a
    call ADC_measure
    bra $-4
    return
   
ADC_measure:
    movlw 0b00100101 ;AN9 a.k.a RB3, ADC on
    movwf ADCON0,a
    bsf	  ADCON0,1,1 ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, ADC_RH 	; Result is complete - store 8 MSbits in ADC_RH
    movff ADRESL, ADC_RL	; store 2 LSbits in ADC_RL
    movff ADRESH, RGB_R
    movff ADRESH, PORTA
    return

exit: