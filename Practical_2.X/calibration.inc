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

calc_threshold_min macro margin,v,rr
    movlw Thres_var
    cpfslt v,a
    bra $+8
    movlw 0
    movwf rr,a
    bra $+10
    movff v,rr
    movlw margin
    subwf rr,a
endm
 
calc_threshold_max macro margin,v,rr
    movlw 255-Thres_var
    cpfsgt v,a
    bra $+8
    movlw 255
    movwf rr,a
    bra $+10
    movff v,rr
    movlw margin
    addwf rr,a
endm
    
Calc_Color_Threshold macro rthres_min, rthres_max, gthres_min, gthres_max, bthres_min, bthres_max
    average_values s1r, s2r, s3r, s4r, s5r, tmp
    calc_threshold_min Thres_var, tmp, rthres_min
    calc_threshold_max Thres_var, tmp, rthres_max
    
    average_values s1g, s2g, s3g, s4g, s5g, tmp
    calc_threshold_min Thres_var, tmp, gthres_min
    calc_threshold_max Thres_var, tmp, gthres_max
 
    average_values s1b, s2b, s3b, s4b, s5b, tmp
    calc_threshold_min Thres_var, tmp, bthres_min
    calc_threshold_max Thres_var, tmp, bthres_max
endm

calibrate:
    call calibrate_start
    return 
    
calibrate_start:
    movlw   0b00000001
    movwf   rcalib,a
    calibrate_for_white:
	movff	rcalib,WREG
	bsf	DUMP_REG,0,a
	btfss	rcalib,1,a 
	bra $-2
	bcf	rcalib,1,a
	call read_Sensor_all
	Calc_Color_Threshold W_R_Thres_min, W_R_Thres_max, W_G_Thres_min, W_G_Thres_max, W_B_Thres_min, W_B_Thres_max
	flash_Reg tmp, 3, DUMP_REG, 0b00000001
    calibrate_for_green:
	movff rcalib,WREG
	bsf	DUMP_REG,1,a
	btfss	rcalib,1,a 
	bra $-2
	bcf	rcalib,1,a
	call read_Sensor_all
	Calc_Color_Threshold G_R_Thres_min, G_R_Thres_max, G_G_Thres_min, G_G_Thres_max, G_B_Thres_min, G_B_Thres_max
	flash_Reg tmp, 3, DUMP_REG, 0b00000010
    calibrate_for_blue:
    	movff rcalib,WREG
	bsf	DUMP_REG,2,a
	btfss	rcalib,1,a 
	bra $-2
	bcf	rcalib,1,a
	call read_Sensor_all
	Calc_Color_Threshold B_R_Thres_min, B_R_Thres_max, B_G_Thres_min, B_G_Thres_max, B_B_Thres_min, B_B_Thres_max
	flash_Reg tmp, 3, DUMP_REG, 0b00000100
    calibrate_for_black:
	movff rcalib,WREG
	bsf	DUMP_REG,4,a
	btfss	rcalib,1,a 
	bra $-2
	bcf	rcalib,1,a
	call read_Sensor_all
	Calc_Color_Threshold K_R_Thres_min, K_R_Thres_max, K_G_Thres_min, K_G_Thres_max, K_B_Thres_min, K_B_Thres_max
	flash_Reg tmp, 3, DUMP_REG, 0b00010000
    calibrate_for_red:
	movff rcalib,WREG
	bsf	DUMP_REG,3,a
	btfss	rcalib,1,a 
	bra $-2
	bcf	rcalib,1,a
	call read_Sensor_all
	Calc_Color_Threshold R_R_Thres_min, R_R_Thres_max, R_G_Thres_min, R_G_Thres_max, R_B_Thres_min, R_B_Thres_max
	flash_Reg tmp, 3, DUMP_REG, 0b00001000
    clrf DUMP_REG,a
    clrf rcalib,a
    return     
    
;========== Tests ==========
   
calibrate_test:
	movlw 250
	movwf s1r,a
	movwf s2r,a
	movwf s3r,a
	movwf s4r,a
	movwf s5r,a
	movlw 250
	movwf s1g,a
	movwf s2g,a
	movwf s3g,a
	movwf s4g,a
	movwf s5g,a
	movlw 250
	movwf s1b,a
	movwf s2b,a
	movwf s3b,a
	movwf s4b,a
	movwf s5b,a
    	Calc_Color_Threshold W_R_Thres_min, W_R_Thres_max, W_G_Thres_min, W_G_Thres_max, W_B_Thres_min, W_B_Thres_max

	movlw 8
	movwf s1r,a
	movwf s2r,a
	movwf s3r,a
	movwf s4r,a
	movwf s5r,a
	movlw 10
	movwf s1g,a
	movwf s2g,a
	movwf s3g,a
	movwf s4g,a
	movwf s5g,a
	movlw 15
	movwf s1b,a
	movwf s2b,a
	movwf s3b,a
	movwf s4b,a
	movwf s5b,a
    	Calc_Color_Threshold K_R_Thres_min, K_R_Thres_max, K_G_Thres_min, K_G_Thres_max, K_B_Thres_min, K_B_Thres_max
	
	movlw 238
	movwf s1r,a
	movwf s2r,a
	movwf s3r,a
	movwf s4r,a
	movwf s5r,a
	movlw 39
	movwf s1g,a
	movwf s2g,a
	movwf s3g,a
	movwf s4g,a
	movwf s5g,a
	movlw 50
	movwf s1b,a
	movwf s2b,a
	movwf s3b,a
	movwf s4b,a
	movwf s5b,a
    	Calc_Color_Threshold R_R_Thres_min, R_R_Thres_max, R_G_Thres_min, R_G_Thres_max, R_B_Thres_min, R_B_Thres_max

	movlw 98
	movwf s1r,a
	movwf s2r,a
	movwf s3r,a
	movwf s4r,a
	movwf s5r,a
	movlw 224
	movwf s1g,a
	movwf s2g,a
	movwf s3g,a
	movwf s4g,a
	movwf s5g,a
	movlw 63
	movwf s1b,a
	movwf s2b,a
	movwf s3b,a
	movwf s4b,a
	movwf s5b,a
    	Calc_Color_Threshold G_R_Thres_min, G_R_Thres_max, G_G_Thres_min, G_G_Thres_max, G_B_Thres_min, G_B_Thres_max

	movlw 30
	movwf s1r,a
	movwf s2r,a
	movwf s3r,a
	movwf s4r,a
	movwf s5r,a
	movlw 68
	movwf s1g,a
	movwf s2g,a
	movwf s3g,a
	movwf s4g,a
	movwf s5g,a
	movlw 204
	movwf s1b,a
	movwf s2b,a
	movwf s3b,a
	movwf s4b,a
	movwf s5b,a
    	Calc_Color_Threshold B_R_Thres_min, B_R_Thres_max, B_G_Thres_min, B_G_Thres_max, B_B_Thres_min, B_B_Thres_max
	return

calibrate_test_cont:
    cont_test_loop:
    call read_Sensor5
    movff s5r,DUMP_REG
    wait_timer H333ms,L333ms
    movff s5g,DUMP_REG
    wait_timer H333ms,L333ms
    movff s5b,DUMP_REG
    wait_timer H333ms,L333ms
    clrf DUMP_REG,a
    bra cont_test_loop
    
calibrate_test_int:
    call read_Sensor1
    movff ADRESH,DUMP_REG
    clrf rcalib,a
    bsf	 rcalib,0,a
    btfss rcalib,1,a
    bra $-2
    bra $-16