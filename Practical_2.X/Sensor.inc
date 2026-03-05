divSensoradd macro numerator,r1,r2
    movlw   numerator
    mulwf   r1,a
    btfsc   PRODL,7,a
    bsf	    PRODH,0,a 
    movff PRODH,WREG
    addwf   r2,a
endm
    
average_values macro r1,r2,r3,r4,r5,rr
    clrf rr,a
    divSensoradd 51, r1, rr
    divSensoradd 51, r2, rr
    divSensoradd 51, r3, rr
    divSensoradd 51, r4, rr
    divSensoradd 51, r5, rr
endm
    
ADC_measure macro r1
    bsf	  ADCON0,1,a ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, r1
endm
    
RGB_measure macro rR, rG, rB
    clrf    RGB_REG,a
    bsf	    RGB_REG,0,a
    ADC_measure rR
    rlcf    RGB_REG,a
    ADC_measure rG
    rlcf    RGB_REG,a
    ADC_measure rB
endm
    
read_Sensor_all:
    call read_Sensor1
    call read_Sensor2
    call read_Sensor3
    call read_Sensor4
    call read_Sensor5
    return
    
read_Sensor1: 
    movlw 0b00111101 ;AN15 a.k.a RC3, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    RGB_measure s1r, s1b, s1g
    return

read_Sensor2: 
    movlw 0b01000001 ;AN16 a.k.a RC4, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    RGB_measure s2r, s2b, s2g
    return

read_Sensor3: 
    movlw 0b01000101 ;AN17 a.k.a RC5, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    RGB_measure s3r, s3b, s3g
    return

read_Sensor4: 
    movlw 0b01001001 ;AN18 a.k.a RC6, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    RGB_measure s4r, s4b, s4g
    return

read_Sensor5: 
    movlw 0b01001101 ;AN19 /.k.a RC7, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL  
    RGB_measure s5r, s5b, s5g
    return