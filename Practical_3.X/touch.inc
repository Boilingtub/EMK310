touch_measure:
    movlb 0xF
    bcf ANSELB,5,1
    bcf TRISB,5,1
    bcf PORTB,5,0
    wait_timer ADCAQTH, ADCAQTL
    bsf  PORTB,5,0
    wait_timer ADCAQTH, ADCAQTL
    bcf PORTB,5,0
    bsf ANSELB,5,1
    bsf TRISB,5,1
    
    wait_timer CAPTH,CAPTL
    
    ADC_measure cap_reg
    
    movlw CAP_THRES
    cpfslt cap_reg,a
    bra $+6
    setf cap_reg,a
    bra $+4
    clrf cap_reg,a
   
    return
    
read_touch:
    movlw 0b00110101 ;AN13 a.k.a RB5, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    call touch_measure
    call touch_led
    return

touch_led:
    tstfsz cap_reg,a
    bra $+6
    bcf PORTA,2,a
    bra $+4
    bsf PORTA,2,a
    return

start_on_touch:
    call read_touch
    tstfsz cap_reg,a
    bra $+4
    bra start_on_touch
    wait_timer H333ms, L333ms
    return
    