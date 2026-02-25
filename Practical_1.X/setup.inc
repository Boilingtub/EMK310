setup:
;Initialize Port A (check Data Sheet)
movlb	0xF	    ;Set BSR for banked SFRs (Bank 15)
clrf	PORTA, a    ; Init port A, clear output of data latches
clrf	LATA, a	    ; Alternate way to clear all data latches
clrf	ANSELA, a   ; Make Digital Inputs
clrf	TRISA, a    ; SET all RA digital Out
  
;Initialize Port B (check Data Sheet)
movlb	0xF	;Set BSR for Banked SFR (Bank 15)
clrf	PORTB,a	;clear output of data latches
clrf	LATB, a	;alternative method
movlw	0b00010001
movwf	ANSELB, a ; RB<7:5,3:1> OUT, RB<4,0> IN
movlw	0b00010001
movwf   TRISB,	a   ;Set RB<7:5,3:1> to Digital, RB<4,1> to Analog

;Initialize Port C (check Data Sheet)
movlb	0xF ;Set BSR for Banked SFR (Bank 15)
clrf	PORTC,a
clrf	LATC,a
movlw	0b00000000
movwf	ANSELC,a    ;Set RD<7:0> to Digital
movlw	0b00000000
movwf	TRISC,a	    ;Set RD<7:0> to Output
    
;Initialize Port D (check Data Sheet)
movlb	0xF ;Set BSR for Banked SFR (Bank 15)
clrf	PORTD,a
clrf	LATD,a
movlw	0b00000000
movwf	ANSELD,a    ;Set RD<7:0> to Digital
movlw	0b00000000
movwf	TRISD,a	    ;Set RD<7:0> to Output
    

;Initialize Timer (Check DataSheet)
bsf OSCCON, 6,a
bcf OSCCON, 5,a
bsf OSCCON, 4,a
bsf OSCCON, 1,a

;Initialize ADC (Check DataSheet)
movlw 0b00101111 ;left justify, Frc, 12 TAD ACQ time
movwf ADCON2,a
movlw 0b00000000 ;ADC ref = Vdd,Vss
movwf ADCON1,a
movlw 0b00110001 ;AN12 a.k.a RB0, ADC on
movwf ADCON0,a

return
