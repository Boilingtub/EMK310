pwm_setup:
    movlb 0xF
    ;Select Timer2 for CCP1,2,3 modules
    banksel    CCPTMRS0
    clrf    CCPTMRS0,1    ; select Timer 2
    ;Set CCP PWM period
    banksel PR2
    ;movlw   0b11111001   ; Sets period for ~1kHz at 16MHz with 1:16 prescale
    movlw 249
    movwf   PR2,1
    ;set CCP Duty cycles
    banksel CCPR2L
    movlw   0x00  ;Duty Cycle for CCP1 (RB3) but off for setup
    movwf   CCPR2L,1
    
    banksel CCPR3L
    movlw   0x00  ;Duty Cycle for CCP2 (RE0) but off for setup
    movwf   CCPR3L,1
    
    banksel CCPR1L
    movlw   0x00  ;Duty Cycle for CCP4 (RD1) off for setup
    movwf   CCPR1L,1
    
    banksel CCPR5L
    movlw   0x00  ;Duty Cycle for CCP5 (RE2) off for setup
    movwf   CCPR5L,1
    ; Set ccp's to standard PWM modes
    movlw   0b00111100 ;0b00111100
    banksel CCP2CON
    movwf   CCP2CON,1
    banksel CCP3CON
    movwf   CCP3CON,1
    banksel CCP1CON
    movwf   CCP1CON,1
    banksel CCP5CON
    movwf   CCP5CON,1
    ;Start Timer 2
    movlw   0b00000100
    movwf   T2CON,1
    ;PheriPheral Module Register
    movlw   0b00001000; (E) CCP5, (D) CCP4, (E) CCP3 (E) CCP2 (E) CCP1
    movwf   PMD1,0
    return

