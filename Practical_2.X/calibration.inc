take_RGB_measurement macro reg_col
    movlw 0b00000001 ;AN0, ADC on
    movwf ADCON0,a
    bsf	  ADCON0,1,a ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, ADC_RH 	; Result is complete - store 2 MSbits in
    movff ADRESL, ADC_RL	; RESULTHI and 8 LSbits in RESULTLO
    movff ADRESL, reg_col
endm
    
ADC_measure macro r1, r2, r3, rled, ival, b0, b1, b2
    movlw 1
    movwf rled,a
    bsf	  ADCON0,1,a ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    btfsc rled,b0,a
    bra $+10	
    btfsc rled,b1,a
    bra $+14
    btfsc rled,b2,a
    bra $+20
    movff ADRESH, r1
    rlcf  rled,a
    bra $-24
    movff ADRESH, r2
    rlcf rled,a
    bra $-32
    movff ADRESH, r3
endm

flash_Reg macro count_addr, count_val, out_reg, out_val
    movlw count_val
    movwf count_addr,a
    movlw out_val			      ;movlw is		 2 bytes
    movwf out_reg,a			      ;movwf is	         2 bytes
    wait_timer H333ms,L333ms		      ;wait_timer is    22 bytes
    clrf  out_reg,a			      ;clrf is		 2 bytes
    wait_timer H333ms,L333ms		      ;wait_timer is	22 bytes
    decfsz  count_addr,a		      ;decfsz is	 2 bytes
    bra	    $-52			      ;put PC		52 bytes back
endm
    

calibrate:
    call calibrate_start
    call calibrate_start
    return 
    
calibrate_start:
    movlw  0b00000001
    movwf rcalib,a
    calibrate_for_white:
	movff	rcalib,WREG
	bsf	DUMP_REG,0,a
	cpfslt	rcalib,a    ;Less than as rclf loops msb
	bra $-2
	call calibrate_RGB
	flash_Reg tmp, 3, DUMP_REG, 0b00000001
    calibrate_for_green:
	movff rcalib,WREG
	bsf	DUMP_REG,1,a
	cpfsgt	rcalib,a
	bra $-2
	call calibrate_RGB
	flash_Reg tmp, 3, DUMP_REG, 0b00000010
    calibrate_for_blue:
    	movff rcalib,WREG
	bsf	DUMP_REG,2,a
	cpfsgt	rcalib,a
	bra $-2
	call calibrate_RGB
	flash_Reg tmp, 3, DUMP_REG, 0b00000100
    calibrate_for_red:
	movff rcalib,WREG
	bsf	DUMP_REG,3,a
	cpfsgt rcalib,a
	bra $-2
	call calibrate_RGB
	flash_Reg tmp, 3, DUMP_REG, 0b00001000
    calibrate_for_black:
	movff rcalib,WREG
	bsf	DUMP_REG,4,a
	cpfsgt	rcalib,a
	bra $-2
	call calibrate_RGB
	flash_Reg tmp, 3, DUMP_REG, 0b00010000
    clrf DUMP_REG,a
    return
        
calibrate_RGB:
    btfsc   rcalib,1,a
    bra calibrate_W
    btfsc   rcalib,2,a
    bra calibrate_G
    btfsc   rcalib,3,a
    bra calibrate_B
    btfsc   rcalib,4,a
    bra calibrate_R
    btfsc   rcalib,5,a
    bra calibrate_K
    calibrate_W:
	call read_Sensor1
    calibrate_G:
    calibrate_B:
    calibrate_R:
    calibrate_K:
    return
    
    
take_ADC_measurement:
    movlw 1
    
    movwf RGB_REG,a
    bsf	  ADCON0,1,a ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    btfsc RGB_REG,0,a
    bra $+10	
    btfsc RGB_REG,1,a
    bra $+14
    btfsc RGB_REG,2,a
    bra $+20
    movff ADRESH, mR
    rlcf  RGB_REG,a
    bra $-24
    movff ADRESH, mG
    rlcf RGB_REG,a
    bra $-32
    movff ADRESH, mB
    return
 
read_Sensor1: 
    movlw 0b00111111 ;AN15 a.k.a RC3, ADC on
    movwf ADCON0,a
    call take_ADC_measurement
    return

read_Sensor2: 
    movlw 0b01000011 ;AN16 a.k.a RC4, ADC on
    movwf ADCON0,a
    call take_ADC_measurement
    return

read_Sensor3: 
    movlw 0b01000111 ;AN17 a.k.a RC5, ADC on
    movwf ADCON0,a
    call take_ADC_measurement
    return

read_Sensor4: 
    movlw 0b0101011 ;AN18 a.k.a RC6, ADC on
    movwf ADCON0,a
    call take_ADC_measurement
    return

read_Sensor5: 
    movlw 0b01001111 ;AN19 a.k.a RC7, ADC on
    movwf ADCON0,a
    call take_ADC_measurement
    return
    
calibrate_test:
    bra calibrate_test_cont
  
calibrate_test_cont:
    call read_Sensor1
    movff ADRESH,DUMP_REG
    bra $-8
    
calibrate_test_int:
    call read_Sensor1
    movff ADRESH,DUMP_REG
    clrf rcalib,a
    bsf	 rcalib,0,a
    btfss rcalib,1,a
    bra $-2
    bra $-16
    
    




