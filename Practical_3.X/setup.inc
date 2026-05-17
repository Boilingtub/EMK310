setup:
movlb	0xF	    ;Set BSR for banked SFRs (Bank 15)
; Clear Registers
    clrf search_sensor
    
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
movlw	0b00100000
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
   
;Setup UART  
    ; Baud rate setup (Datasheet RX#1)
    MOVLW   51			; 19230 BAUD @ 16 MHz
				; table 18-5 of datasheet
    MOVWF   SPBRG2	  	; load baudrate register
    CLRF    SPBRGH2
    movlw 0b00000000
    movwf BAUDCON2
    movlw 0b00100100
    movwf TXSTA2
    movlw 0b10010000
    movwf RCSTA2
        
;Setup I2C  !!!! NEEDS CHANGE !!!!
    ; Set up the BAUD rate to 100 kHz
    MOVLW   00001001B		;BRG value from Table 15-3 for 100 kHz 
    MOVWF   SSP1ADD	     
    
    ; Set up MSSP status register
    CLRF    SSP1STAT
    BSF	    SSP1STAT, 7	   
    
    ; MSSP Control register 1 (SSPxCON1) 
    ;   used to enable the MSSP peripheral and set the device in I2C Master mode    
    MOVLW   00101000B
    MOVWF   SSP1CON1
    
    ; MSSP Control register 2 (SSPxCON2) 
    ;	used to send the Start and Stop conditions, set the Receive mode and
    ;	handle the Acknowledge bits
    CLRF    SSP1CON2
    
    ; Clear interrupt flags
    BCF	    SSP1IF
    BCF	    BCL1IF

    
;Enable CCP modules
call pwm_setup
    
;Setup Interrupts    
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

; set up interrupts for UART
BCF    RC2IF			; Clear RCIF Interrupt Flag
BSF    RC2IE			; Set RCIE Interrupt Enable (Datasheet RX#4)    

;Enable Interrupts
movf  PORTB, 0,1	; Read Port B (clear mismatch)
bcf   INTCON, 0, 1     ; Clear RBIF flag (bit 0)
bsf   INTCON, 3, 1     ; Enable RBIE (Port B change interrupt, bit 3)
bsf   INTCON, 7, 1      ; Enable GIE (Global interrupt, bit 7)
bsf PEIE
    
movlb 0x00
return



