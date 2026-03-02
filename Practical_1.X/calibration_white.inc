take_RGB_measurement macro reg_col
    movlw 0b00000001 ;AN0, ADC on
    movwf ADCON0,a
    bsf	  ADCON0,1,a ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, ADC_RH 	; Result is complete - store 2 MSbits in
endm
    
ADC_measure macro r1
    bsf	  ADCON0,1,a ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, r1
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
    
TEST_FLASH macro count_addr, count_val, out_reg, out_val
    movlw count_val
    movwf count_addr,a
    movlw out_val			      ;movlw is		 2 bytes
    movwf out_reg,a			      ;movwf is	         2 bytes
    wait_timer ADCAQTH,ADCAQTL		      ;wait_timer is    22 bytes
    clrf  out_reg,a			      ;clrf is		 2 bytes
    wait_timer ADCAQTH,ADCAQTL		      ;wait_timer is	22 bytes
    decfsz  count_addr,a		      ;decfsz is	 2 bytes
    bra	    $-52			      ;put PC		52 bytes back
endm
    
fake_read_Sensor macro rr,val
    movlw 0xFF
    movwf rr
endm
 
divSensoradd macro numerator,r1,r2
    movlw   numerator
    mulwf   r1,a
    btfsc   PRODL,7,a
    bsf	    PRODH,0,a 
    movff PRODH,WREG,a
    addwf   r2,a
endm
    
calc_middle macro rl,rh,rr
    movff rl,WREG,a
    movff rh,tmp,a
    subwf tmp,a
    rrncf  tmp,a
    bcf   tmp,7,a
    movff tmp,WREG,a
    movff rl,rr
    addwf rr,a
endm
 
check_navline macro sv, col_reg, rr, bit 
    movff nav_col,WREG
    determine_color sv,col_reg
    CPFSEQ col_reg,a
    bra $+4
    bsf rr,bit,a
endm
    
determine_color macro rt,rr
 
    movlw  4
    movwf  rr,a
    
    movff W_thres,WREG,a
    CPFSLT rt,a
    bra $+34 ;ret W
   
    decf rr,a
    movff G_thres,WREG,a
    cpfslt rt,a
    bra $+24 ; ret G
    
    decf rr,a
    movff B_thres,WREG,a
    cpfslt rt,a
    bra $+14 ;ret B
    
    decf rr,a
    movff R_thres,WREG,a
    cpfslt rt,a
    bra $+4 ; ret R
    
    decf rr,a
    ; ret K
    
 endm
    
calibrate:
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
    ;call calc_ranges
    call calc_discrete_ranges
    clrf DUMP_REG,a
    clrf rcalib,a
    return
        
calibrate_RGB:
    call read_Sensor_all
    call average_Sensor_Values
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
	movff SVAvg,mW
	return
    calibrate_G:	
	movff SVAvg,mG
	return
    calibrate_B:
    	movff SVAvg,mB
	return
    calibrate_R:
    	movff SVAvg,mR
	return
    calibrate_K:
    	movff SVAvg,mK
	return
     
read_Sensor_all:
    call read_Sensor1
    call read_Sensor2
    call read_Sensor3
    call read_Sensor4
    call read_Sensor5
    return
 
average_Sensor_Values:
    clrf SVAvg,a
    divSensoradd 51,SV1, SVAvg
    divSensoradd 51,SV2, SVAvg
    divSensoradd 51,SV3, SVAvg
    divSensoradd 51,SV4, SVAvg
    divSensoradd 51,SV5, SVAvg
    ;movff SV3,SVAvg,a
    return
    
calc_ranges:
    calc_middle mG,mW,W_thres
    calc_middle mB,mG,G_thres	
    calc_middle mR,mB,B_thres
    calc_middle mK,mR,R_thres
    ;set K thres to 0
    movlw 0
    movwf K_thres,a
    return
    calc_discrete_ranges:
	bra calc_ranges
	clrf WREG,a
	clrf tmp,a
        movff mG,WREG,a
	movff mW,tmp,a
	subwf tmp,a
	movff tmp,WREG,a
	rrncf  WREG,a
	bcf WREG,7,a
	movff mG,W_thres
	addwf W_thres,a

	clrf WREG,a
	clrf tmp,a
	movff mB,WREG,a
	movff mG,tmp,a
	subwf tmp,a
	movff tmp,WREG,a
	rrcf  tmp,a
	movff mB,G_thres
	addwf G_thres,a
	return
    
 
read_Sensor1: 
    movlw 0b00111101 ;AN15 a.k.a RC3, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    ADC_measure SV1
    return

read_Sensor2: 
    movlw 0b01000001 ;AN16 a.k.a RC4, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    ADC_measure SV2
    return

read_Sensor3: 
    movlw 0b01000101 ;AN17 a.k.a RC5, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    ADC_measure SV3
    return

read_Sensor4: 
    movlw 0b01001001 ;AN18 a.k.a RC6, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    ADC_measure SV4
    return

read_Sensor5: 
    movlw 0b01001101 ;AN19 /.k.a RC7, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    ADC_measure SV5
    return
    
det_col_LED:
    clrf r_col_det,a
    bsf r_col_det,0,a
    movlw 4
   
    cpfseq  rcolor,a
    bra $+4
    bra $+10
    rlncf   r_col_det,a
    decf WREG,a
    btfss  r_col_det,4,a
    bra $-12
    movff r_col_det,DUMP_REG
    return

    
   
Sensor_LLI_Generate:
    clrf Sensor,a
    clrf tmp,a
    call read_Sensor_all
    get_LLI_color:
	 
	
	determine_color SV1,rcolor
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,0,a
	
	determine_color SV2,rcolor
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,1,a
	
	determine_color SV3,rcolor
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,2,a
	
	determine_color SV4,rcolor
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,3,a
	
	determine_color SV5,rcolor
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,4,a
	
	movlw 5
	cpfseq tmp,a
	bra $+8
	movlw 0b00011111
	movwf PORTA,a
	goto stop
	;check_navline SV1,rcolor,Sensor,0
	;check_navline SV2,rcolor,Sensor,1
	;check_navline SV3,rcolor,Sensor,2	
	;check_navline SV4,rcolor,Sensor,3
	;check_navline SV5,rcolor,Sensor,4
	clrf PORTA,a
	movff Sensor,PORTA
	goto left_logic
    return
	
check_navline_discrete:
    
    
    
    
    
    
;========== Tests ==========   
detect_color_test:
    call read_Sensor3
    ;determine_color SV3,rcolor
    call determine_color_test
    call det_col_LED
    return
    
   
average_Sensor_Values_test:
    movlw 50
    movwf SV1,a
    movlw 25
    movwf SV2,a
    movlw 50
    movwf SV3,a
    movlw 50
    movwf SV4,a
    movlw 15
    movwf SV5,a
    call average_Sensor_Values
    return
   
calc_ranges_test:
    movlw 254
    movwf mW,a
    movlw 192
    movwf mG,a
    movlw 128
    movwf mB,a
    movlw 63
    movwf mR,a
    movlw 0
    movwf mK,a
    call calc_ranges
    return
    
    
calc_ranges_test_2:
    movlw 252
    movwf mW,a
    movlw 185
    movwf mG,a
    movlw 131
    movwf mB,a
    movlw 76
    movwf mR,a
    movlw 17
    movwf mK,a
    call calc_discrete_ranges
    return
  
fake_sensor_LLI_test:
    call calc_ranges_test
    movlw 45
    movwf SV1,a
    movlw 89
    movwf SV2,a
    movlw 109
    movwf SV3,a
    movlw 77
    movwf SV4,a
    movlw 123
    movwf SV5,a
    movlw 1
    movwf nav_col,a
    call Sensor_LLI_Generate
    return
    
fake_color_detect_test:
    call calc_ranges_test
    
    movlw  255
    movwf  SV1,a
    determine_color SV1,rcolor
    call det_col_LED
    ;call determine_color_test
    nop
    
    movlw  191
    movwf  SV1,a
    determine_color SV1,rcolor
    call det_col_LED
    ;call determine_color_test
    nop
    
    movlw  127
    movwf  SV1,a
    determine_color SV1,rcolor
    call det_col_LED
    ;call determine_color_test
    nop
    
    movlw  64
    movwf  SV1,a
    determine_color SV1,rcolor
    call det_col_LED
    ;call determine_color_test
    nop
    
    movlw  23
    movwf  SV1,a
    call determine_color_test
    nop
    
    return
    
determine_color_test:
    movlw  4
    movwf  rcolor,a
    
    movff W_thres,WREG,a
    CPFSLT SV3,a
    bra $+34 ;ret W
   
    decf rcolor,a
    movff G_thres,WREG,a
    cpfslt SV3,a
    bra $+24 ; ret G
    
    decf rcolor,a
    movff B_thres,WREG,a
    cpfslt SV3,a
    bra $+14 ;ret B
    
    decf rcolor,a
    movff R_thres,WREG,a
    cpfslt SV3,a
    bra $+4 ; ret R
    
    decf rcolor,a
    ; ret K
    return
    
calibrate_test:
    bra calibrate_test_cont
  
calibrate_test_cont:
    movlw 0b01000101 ;AN17 a.k.a RC5, ADC on
    movwf ADCON0,a
    wait_timer ADCAQTH,ADCAQTL
    movlw 0b00000111
    movwf RGB_REG,a
    cont_test_loop:
    call read_Sensor3 
    ;ADC_measure SV3
    movff ADRESH,DUMP_REG
    bra cont_test_loop
    
calibrate_test_int:
    call read_Sensor1
    movff ADRESH,DUMP_REG
    clrf rcalib,a
    bsf	 rcalib,0,a
    btfss rcalib,1,a
    bra $-2
    bra $-16
    
    
    
    





