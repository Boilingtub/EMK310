fake_RGB_measure macro rR, vR, rG, vG ,rB, vB
    movlw   vR
    movwf   rR
    movlw   vG
    movwf   rG
    movlw   vB
    movwf   rB
endm
    
divSensoradd macro numerator,r1,r2
    movlw   numerator
    mulwf   r1,a
    btfsc   PRODL,7,a
    bsf	    PRODH,0,a 
    movff PRODH,WREG
    addwf   r2,a
endm
    
average_values macro r1,r2,r3,r4,r5,rr
    ;clrf rr,a
    ;divSensoradd 51, r1, rr
    ;divSensoradd 51, r2, rr
    ;divSensoradd 51, r3, rr
    ;divSensoradd 51, r4, rr
    ;divSensoradd 51, r5, rr
    movff   r5,rr,a
endm
    
ADC_measure macro r1
    ;wait_timer 0x01,0x01
    bsf	  ADCON0,1,a ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, r1
endm
    
RGB_measure macro rR, rG, rB    
    movlw 6
    movwf RGB_REG,a
    ADC_measure rR
    movlw 5
    movwf RGB_REG,a
    ADC_measure rG
    movlw 3
    movwf   RGB_REG,a
    ADC_measure rB
    movlw 0
    movwf RGB_REG,a
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
    RGB_measure s1r, s1g, s1b
    return

read_Sensor2: 
    movlw 0b01000001 ;AN16 a.k.a RC4, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    RGB_measure s2r, s2g, s2b
    return

read_Sensor3: 
    movlw 0b01000101 ;AN17 a.k.a RC5, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    RGB_measure s3r, s3g, s3b
    return

read_Sensor4: 
    movlw 0b01001001 ;AN18 a.k.a RC6, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    RGB_measure s4r, s4g, s4b
    return

read_Sensor5: 
    movlw 0b01001101 ;AN19 /.k.a RC7, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL  
    RGB_measure s5r, s5g, s5b
    return