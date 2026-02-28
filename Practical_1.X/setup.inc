setup:
;Initialize Port A (check Data Sheet)
movlb	0xF	    ;Set BSR for banked SFRs (Bank 15)
clrf	PORTA, a    ; Init port A, clear output of data latches
clrf	LATA, a	    ; Alternate way to clear all data latches
clrf	ANSELA, a   ; Make Digital Inputs
clrf	TRISA, a    ; SET all RA digital Out
  
;Initialize Port B (check Data Sheet)
movlb	0xF	;Set BSR for Banked SFR (Bank 15)
clrf	PORTB,b	;clear output of data latches
clrf	LATB, b	;alternative method
movlw	0b00100000
movwf	ANSELB, b ;Set RB<7:5,3:0> to Digital, RB<4> to Analog
movlw	0b00110000
movwf   TRISB,	b       ; RB<7:6,3:0> OUT, RB<5,4> IN

;Initialize Port C (check Data Sheet)
movlb	0xF ;Set BSR for Banked SFR (Bank 15)
clrf	PORTC,a
clrf	LATC,a
movlw	0b00001000
movwf	ANSELC,a    ;Set RD<7:0> to Digital
movlw	0b00001000
movwf	TRISC,a	    ;Set RD<7:0> to Output
    
;Initialize Port D (check Data Sheet)
movlb	0xF ;Set BSR for Banked SFR (Bank 15)
clrf	PORTD,a
clrf	LATD,a
movlw	0b00000000
movwf	ANSELD,a    ;Set RD<7:0> to Digital
movlw	0b00000000
movwf	TRISD,a	    ;Set RD<7:0> to Output
    
;Initialize Port E (check Data Sheet)
movlb	0xF ;Set BSR for Banked SFR (Bank 15)
clrf	PORTE,a
clrf	LATE,a
clrf	ANSELE,a    ;Set RE<7:0> to Digital
clrf	TRISE,a	    ;Set RE<7:0> to Output
    
    
;Initialize Timer (Check DataSheet)
movlw 0b01110100
movwf OSCCON,a

;Initialize ADC (Check DataSheet)
movlw 0b00110010 ;left justify, Frc, 12 TAD ACQ time
movwf ADCON2,a
movlw 0b00000000 ;ADC ref = Vdd,Vss
movwf ADCON1,a
movlw 0b00111111 ;AN15 a.k.a RC3, ADC on
movwf ADCON0,a

    
    
;Setup ADC on complete interrupt
bsf PIE1,6,b
bsf IPR1,6,b
    
;Configure Pull-ups and IOC (Bank 15)
bsf   WPUB, 4, b	; Enable weak pull-up for RB4
bsf   IOCB, 4, b        ; Enable Interrupt-on-change for RB4
;bsf   WPUB, 5, b	; Enable weak pull-up for RB5
bsf   IOCB, 5, b       ; Enable Interrupt-on-change for RB5

;Global Pull-up Enable
bcf   INTCON2, 7, a  ;Global RBPU enable (0 = ON)
    
;Enable Interrupts
movf  PORTB, w, a	; Read Port B (clear mismatch)
bcf   INTCON, 0, a     ; Clear RBIF flag (bit 0)
bsf   INTCON, 3, a     ; Enable RBIE (Port B change interrupt, bit 3)
bsf   INTCON, 7, a      ; Enable GIE (Global interrupt, bit 7)
    
movlb 0x00
return
