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
movlw	0b00010000
movwf	ANSELB, a ;Set RB<7:5,3:0> to Digital, RB<4> to Analog
movlw	0b00110000
movwf   TRISB,	a       ; RB<7:6,3:0> OUT, RB<5,4> IN

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

;Initialize ADC (Check DataSheet)
movlw 0b00101111 ;left justify, Frc, 12 TAD ACQ time
movwf ADCON2,a
movlw 0b00000000 ;ADC ref = Vdd,Vss
movwf ADCON1,a
movlw 0b00101101 ;AN11 a.k.a RB4, ADC on
movwf ADCON0,a
    
;Configure Pull-ups and IOC (Bank 15)
bsf   WPUB, 4, a	; Enable weak pull-up for RB4
bsf   IOCB, 4, a        ; Enable Interrupt-on-change for RB4
bsf   WPUB, 5, a	; Enable weak pull-up for RB5
bsf   IOCB, 5, a        ; Enable Interrupt-on-change for RB5

;Global Pull-up Enable
bcf   INTCON2, 7, a  ;Global RBPU enable (0 = ON)
    
;Enable Interrupts
movf  PORTB, w, a	; Read Port B (clear mismatch)
bcf   INTCON, 0, a     ; Clear RBIF flag (bit 0)
bsf   INTCON, 3, a     ; Enable RBIE (Port B change interrupt, bit 3)
bsf   INTCON, 7, a      ; Enable GIE (Global interrupt, bit 7)
return
