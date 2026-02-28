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
    
tune_LED macro req_val, LED_reg, PWM_reg, ret_lbl
    movlb 0xf
    call read_and_average_sensors
    movlw   req_val
    cpfseq LED_reg,a
    bra $+4
    bra ret_lbl
    cpfslt LED_reg,a
    bra $+4
    incf PWM_reg,a
    cpfsgt LED_reg,a
    bra $-10
    decf PWM_reg,a
    bra $-12
    movlb 0x0
endm

pwm_setup:
    movlb 0xF
    ;Select Timer2 for CCP1,2,3 modules
    banksel	CCPTMRS0
    clrf    CCPTMRS0,b    ; select Timer 2
    ;Set CCP PWM period
    banksel PR2
    ;movlw   0b11111001   ; Sets period for ~1kHz at 16MHz with 1:16 prescale
    movlw 64
    movwf   PR2,b
    ;set CCP Duty cycles    
    movlw   0x2    ; Duty Cycle for CCP1 (RC1)
    movwf   CCPR1L,b
    movlw   0x2    ; Duty Cycle for CCP2 (RC2)
    movwf   CCPR2L,b
    banksel CCPR3L
    movlw   0x2   ; Duty Cycle for CCP3 (RE0)
    movwf   CCPR3L,b
    ; Set ccp's to standard PWM modes
    movlw   0b00111100	
    movwf   CCP1CON,b
    movwf   CCP2CON,b
    banksel CCP3CON
    movwf   CCP3CON,b
    ;Start Timer 2
    movlw   0b00000100
    movwf   T2CON,b
    movlb 0
    return
    
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
;   calibrate_for_black:
;	movff rcalib,WREG
;	bsf	DUMP_REG,4,a
;	cpfsgt	rcalib,a
;	bra $-2
;	call calibrate_RGB
;	flash_Reg tmp, 3, DUMP_REG, 0b00010000
    clrf DUMP_REG,a
    return
        
calibrate_RGB:
    btfsc   rcalib,1,a
    bra calibrate_W
    btfsc   rcalib,2,a
    ;tune_LED 192, SVAvg, CCPR2L, tune_done ;Tune Green
    call tune_LED_G
    btfsc   rcalib,3,a
    tune_LED 64, SVAvg, CCPR3L, tune_done ;Tune Blue
    ;call tune_LED_B
    btfsc   rcalib,4,a
    tune_LED 128, SVAvg, CCPR1L, tune_done ;Tune Red 
    ;call tune_LED_R
    btfsc   rcalib,5,a
    bra calibrate_K
    calibrate_W:
	movlw 253
	call read_and_average_sensors
	cpfslt SVAvg,a
	return
	call inc_RGB	
	bra calibrate_W
    calibrate_K:
	movlw	16
	call read_and_average_sensors
	cpfsgt SVAvg,a
	return
	call dec_RGB
	bra calibrate_W
    tune_done:
	return
    
tune_LED_G:
    movlb 0xf
    call read_and_average_sensors
    movlw   192
    cpfseq SVAvg,a
    bra $+8
    return
    cpfslt SVAvg,a
    bra $+14
    incf CCPR2L,a
    bra $-18
    cpfsgt SVAvg,a
    bra $-20
    decf CCPR2L,a
    bra $-24
    movlb 0x0
    return
    
tune_LED_B:
    call read_and_average_sensors
    movlw   64
    cpfseq CCPR3L,a
    bra $+4
    return
    cpfslt SVAvg,a
    bra $+4
    incf CCPR3L,a
    cpfsgt SVAvg,a
    bra $-10
    decf CCPR3L,a
    bra $-12
    return

tune_LED_R:
    call read_and_average_sensors
    movlw   128
    cpfseq CCPR1L,a
    bra $+4
    return
    cpfslt SVAvg,a
    bra $+4
    incf CCPR1L,a
    cpfsgt SVAvg,a
    bra $-10
    decf CCPR1L,a
    bra $-12
    return
    
inc_RGB:
    movlb 0xf
    incf CCPR1L,a
    incf CCPR2L,a
    incf CCPR3L,a
    movlb 0x0
    return

dec_RGB:
    movlb 0xf
    decf CCPR1L,a
    decf CCPR2L,a
    decf CCPR3L,a
    movlb 0x0
    return
    
read_and_average_sensors:
    call read_Sensor1
    call average_Sensor_All
    return
    
average_Sensor_All:
    movff SV1,SVAvg,a
    return
    
read_SensorAll:
 call read_Sensor1
 call read_Sensor2
 call read_Sensor3
 call read_Sensor4
 call read_Sensor5
 
read_Sensor1: 
    movlw 0b00111111 ;AN15 a.k.a RC3, ADC on
    movwf ADCON0,a
    call ADC_measure
    movff ADRESH,SV1,a
    return

read_Sensor2: 
    movlw 0b01000011 ;AN16 a.k.a RC4, ADC on
    movwf ADCON0,a
    call ADC_measure
    movff ADRESH,SV2,a
    return

read_Sensor3: 
    movlw 0b01000111 ;AN17 a.k.a RC5, ADC on
    movwf ADCON0,a
    call ADC_measure
    movff ADRESH,SV3,a
    return

read_Sensor4: 
    movlw 0b0101011 ;AN18 a.k.a RC6, ADC on
    movwf ADCON0,a
    call ADC_measure
    movff ADRESH,SV4,a
    return

read_Sensor5: 
    movlw 0b01001111 ;AN19 a.k.a RC7, ADC on
    movwf ADCON0,a
    call ADC_measure
    movff ADRESH,SV5,a
    return
    
calibrate_test:
    bra calibrate_test_cont
calibrate_test_cont:
    call ADC_measure
    movff ADRESH,DUMP_REG
    bra $-8
    
calibrate_test_int:
    call ADC_measure
    movff ADRESH,DUMP_REG
    clrf rcalib,a
    bsf	 rcalib,0,a
    btfss rcalib,1,a
    bra $-2
    bra $-16
    
ADC_measure:
    bsf	  ADCON0,1,a ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, ADC_RH	; Result is complete - store 8 MSbits in ADC_RH
    movff ADRESL, ADC_RL	; store 2 LSbits in ADC_RL
    return
    

