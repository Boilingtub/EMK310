setup:
;Initialize Port A (check Data Sheet)
movlb	0xF	    ;Set BSR for banked SFRs
clrf	PORTA,	    ; Init port A, clear output of data latches
clrf	LATA, a	    ; Alternate way to clear all data latches
clrf	ANSELA, b   ; Make Digital Inputs
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

;Initialize Timer (Check DataSheet)
bsf OSCCON, 6,c
bcf OSCCON, 5,c
bsf OSCCON, 4,c

;Initialize ADC (Check DataSheet)
movlw 0b00101111 ;left justify, Frc, 12 TAD ACQ time
movwf ADCON2,a
movlw 0b00000000 ;ADC ref = Vdd,Vss
movwf ADCON1,a
movlw 0b00100101 ;AN9 a.k.a RB3, ADC on
movwf ADCON0,a
 





