pwm_setup:
    movlb 0xF
    ;Select Timer2 for CCP1,2,3 modules
    banksel    CCPTMRS0
    clrf    CCPTMRS0,b    ; select Timer 2
    ;Set CCP PWM period
    banksel PR2
    ;movlw   0b11111001   ; Sets period for ~1kHz at 16MHz with 1:16 prescale
    movlw 249
    movwf   PR2,b
    ;set CCP Duty cycles
    banksel CCPR2L
    movlw   0x00  ;Duty Cycle for CCP1 (RB3) but off for setup
    movwf   CCPR2L,b
    
    banksel CCPR3L
    movlw   0x00  ;Duty Cycle for CCP2 (RE0) but off for setup
    movwf   CCPR3L,b
    
    banksel CCPR4L
    movlw   0x00  ;Duty Cycle for CCP4 (RD1) off for setup
    movwf   CCPR4L,b
    
    banksel CCPR5L
    movlw   0x00  ;Duty Cycle for CCP5 (RE2) off for setup
    movwf   CCPR5L,b
    ; Set ccp's to standard PWM modes
    movlw   0b00111100 ;0b00111100
    banksel CCP2CON
    movwf   CCP2CON,b
    banksel CCP3CON
    movwf   CCP3CON,b
    banksel CCP4CON
    movwf   CCP4CON,b
    banksel CCP5CON
    movwf   CCP5CON,b
    ;Start Timer 2
    movlw   0b00000100
    movwf   T2CON,b
    ;PheriPheral Module Register
    movlw   0b00000001; (E) CCP5, (E) CCP4, (E) CCP3 (E) CCP2 (D) CCP1
    movwf   PMD1,a
    return

