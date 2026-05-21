setup:
movlb	0xF	    ;Set BSR for banked SFRs (Bank 15)
; Clear Registers
clrf prev_sensor
    
;Initialize Port A (check Data Sheet)
clrf	PORTA	    ; Init port A, clear output of data latches
clrf	LATA	    ; Alternate way to clear all data latches
movlw	0b00000000
movwf	ANSELA,1
movlw	0b00000000
movwf	TRISA,1
    
  
;Initialize Port B (check Data Sheet)
clrf	PORTB	;clear output of data latches
clrf	LATB
movlw	0b00100000
movwf	ANSELB,1
movlw	0b11100110
movwf   TRISB,	1      
    

;Initialize Port C (check Data Sheet)
clrf	PORTC
clrf	LATC
movlw	0b11100000
movwf	ANSELC,1    
movlw	0b11111000
movwf	TRISC,1   
    
;Initialize Port D (check Data Sheet)
clrf	PORTD
clrf	LATD
movlw	0b00001100
movwf	ANSELD,1    
movlw	0b11001100
movwf	TRISD,1   
    
;Initialize Port E (check Data Sheet)
clrf	PORTE
clrf	LATE
movlw	0b00000000
movwf	ANSELE,0   
movlw	0b00000000
movwf	TRISE,0    
    
    
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
    
;Enable CCP modules
    ;Select Timer2 for CCP1,2,3 modules
    clrf    CCPTMRS0,1    ; select Timer 2
    ;Select Timer 2 for CCP 4 and 5
    clrf    CCPTMRS1,1
    ;Set CCP PWM period
    movlw   249
    movwf   PR2,1
    ;set CCP Duty cycles
    movlw   0x00  ;Duty Cycle for CCP1 (RB3) but off for setup
    movwf   CCPR2L,1
    
    movlw   0x00  ;Duty Cycle for CCP2 (RE0) but off for setup
    movwf   CCPR3L,1
    
    movlw   0x00  ;Duty Cycle for CCP4 (RD1) off for setup
    movwf   CCPR4L,1
    
    movlw   0x00  ;Duty Cycle for CCP5 (RE2) off for setup
    movwf   CCPR5L,1
    ; Set ccp's to standard PWM modes
    movlw   0b00111100 ;0b00111100
    movwf   CCP2CON,1
    movwf   CCP3CON,1
    movwf   CCP4CON,1
    movwf   CCP5CON,1
    ;Start Timer 2
    movlw   0b00000100
    movwf   T2CON,1
    ;PheriPheral Module Register
    movlw   0b00000001; (E) CCP5, (E) CCP4, (E) CCP3 (E) CCP2 (D) CCP1
    movwf   PMD1,0
    
    ;Enable Interrupts
    bsf   INTCON, 7, 1      ; Enable GIE (Global interrupt, bit 7)
    bsf PEIE
    
movlb 0x00
return



