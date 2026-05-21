ADC_measure macro r1
    bsf	  ADCON0,1,0 ;Start conversion
    btfsc ADCON0,1,0 ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, r1
endm
    
RGB_measure macro rR, rG, rB   
    ;Due to Tansistor, LED is inverted logic
    bsf PORTC,2 ;LEDB
    bsf PORTC,1 ;LEDG
    
    bcf PORTC,0 ;LEDR
    ADC_measure rR
    bsf PORTC,0 ;LEDR
    
    bcf PORTC,1 ;LEDG
    ADC_measure rG
    bsf PORTC,1 ;LEDG
    
    bcf PORTC,2 ;LEDB
    ADC_measure rB
    bsf PORTC,2 ;LEDB
endm
    
read_Sensor_all:    
    read_Sensor1: 
	movlw 0b01001101 ;AN19 a.k.a RC7, ADC on
	movwf ADCON0,0
	RGB_measure s1r, s1g, s1b

    read_Sensor2: 
	movlw 0b01001001 ;AN18 a.k.a RC6, ADC on
	movwf ADCON0,0
	RGB_measure s2r, s2g, s2b

    read_Sensor3: 
	movlw 0b01000101 ;AN17 a.k.a RC5, ADC on
	movwf ADCON0,0
	RGB_measure s3r, s3g, s3b

    read_Sensor4: 
	movlw 0b01011101 ;AN23 a.k.a RD3, ADC on
	movwf ADCON0,0
	RGB_measure s4r, s4g, s4b

    read_Sensor5: 
	movlw 0b01011001 ;AN22 a.k.a RD2, ADC on
	movwf ADCON0,0
	RGB_measure s5r, s5g, s5b
    return