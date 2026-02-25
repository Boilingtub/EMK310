setup:
;Initialize Port A (check Data Sheet)
movlb	0xF	    ;Set BSR for banked SFRs (Bank 15)
clrf	PORTA,	    ; Init port A, clear output of data latches
clrf	LATA, a	    ; Alternate way to clear all data latches
clrf	ANSELA, b   ; Make Digital Inputs
clrf	TRISA, a    ; SET all RA digital Out
  
;Initialize Port B (check Data Sheet)
movlb	0xF	;Set BSR for Banked SFR (Bank 15)
clrf	PORTB,a	;clear output of data latches
clrf	LATB, a	;alternative method
movlw	0b00001000
movwf	ANSELB, a ; RB<7:4,2:0> OUT, RB<3> IN
movlw	0b00001000
movwf   TRISB,	a   ;Set RB<0:2,4:7> to Digital, RB<3> to Analog
    
;Initialize Port D (check Data Sheet)
movlb	0xF ;Set BSR for Banked SFR (Bank 15)
clrf	PORTD,a
clrf	LATD,a
movlw	0b00000000
movwf	ANSELD,a    ;Set RD<7:0> to Digital
movlw	0b00000000
movwf	TRISD,a	    ;Set RD<7:0> to Output
    
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
 





