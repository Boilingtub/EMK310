setup:
;Initialize Port A (check Data Sheet)
movlb	0xF	    ;Set BSR for banked SFRs (Bank 15)
clrf	PORTA, 1    ; Init port A, clear output of data latches
clrf	LATA, 1	    ; Alternate way to clear all data latches
movlw	0b00000000
movwf	ANSELA,1
movlw	0b00000011
movwf	TRISA,1
    
  
;Initialize Port B (check Data Sheet)
movlb	0xF	;Set BSR for Banked SFR (Bank 15)
clrf	PORTB,1	;clear output of data latches
clrf	LATB, 1	;alternative method
movlw	0b00100000
movwf	ANSELB, 1 ;Set RB<7:5,3:0> to Digital, RB<4> to Analog
movlw	0b00110110
movwf   TRISB,	1       ; RB<7:6,3:0> OUT, RB<5,4> IN
    

;Initialize Port C (check Data Sheet)
movlb	0xF ;Set BSR for Banked SFR (Bank 15)
clrf	PORTC,1
clrf	LATC,1
movlw	0b11111000
movwf	ANSELC,1    ;Set RD<7:0> to Digital
movlw	0b11111000
movwf	TRISC,1   ;Set RD<7:0> to Output
    
;Initialize Port D (check Data Sheet)
movlb	0xF ;Set BSR for Banked SFR (Bank 15)
clrf	PORTD,1
clrf	LATD,1
movlw	0b00000000
movwf	ANSELD,1    ;Set RD<7:0> to Digital
movlw	0b00000000
movwf	TRISD,1    ;Set RD<7:0> to Output
    
;Initialize Port E (check Data Sheet)
movlb	0xF ;Set BSR for Banked SFR (Bank 15)
clrf	PORTE,0
clrf	LATE,0
movlw	0b00000000
movwf	ANSELE,0    ;Set RE<7:0> to Digital
movlw	0b00000000
movwf	TRISE,0    ;Set RE<7:0> to Output
    
    
;Initialize Timer (Check DataSheet)
movlw 0b01110100
movwf OSCCON,0
;movlw 0b00000000
;movwf OSCCON2,a

;Initialize ADC (Check DataSheet)
movlw	0b00111010 ; left Justify 20 TAD, FOSC/32, 
;movlw	0b00111101 ; left Justify 20 TAD, FOSC/16, 
movwf ADCON2,0
movlw 0b00000000 ;ADC ref = Vdd,Vss    
movwf ADCON1,0
movlw 0b00111101 ;AN15 a.k.a RC3, ADC on
movwf ADCON0,0
   
;Setup ADC on complete interrupt
bsf PIE1,6,1
bsf IPR1,6,1
    
;Configure Pull-ups and IOC (Bank 15)
bsf   WPUB, 4, 1	; Enable weak pull-up for RB4
bsf   IOCB, 4, 1        ; Enable Interrupt-on-change for RB4
;bsf   WPUB, 5, b	; Enable weak pull-up for RB5
bsf   IOCB, 5, 1       ; Enable Interrupt-on-change for RB5

;Global Pull-up Enable
bcf   INTCON2, 7, 0  ;Global RBPU enable (0 = ON)
    
;Enable Interrupts
movf  PORTB, 0,1	; Read Port B (clear mismatch)
bcf   INTCON, 0, 1     ; Clear RBIF flag (bit 0)
bsf   INTCON, 3, 1     ; Enable RBIE (Port B change interrupt, bit 3)
bsf   INTCON, 7, 1      ; Enable GIE (Global interrupt, bit 7)

call pwm_setup
    
movlb 0x00
return



