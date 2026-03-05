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
    movlw 33
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
    movlw 222
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
    call read_Sensor_all
    average_values s1r, s2r, s3r, s4r, s5r, tmp
    calc_threshold_min 32, tmp, rthres_min
    calc_threshold_max 32, tmp, rthres_max
    
    average_values s1g, s2g, s3g, s4g, s5g, tmp
    calc_threshold_min 32, tmp, gthres_min
    calc_threshold_max 32, tmp, gthres_max
 
    average_values s1b, s2b, s3b, s4b, s5b, tmp
    calc_threshold_min 32, tmp, bthres_min
    calc_threshold_max 32, tmp, bthres_max
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
	btfss	rcalib,0,a 
	bra $-2
	bcf	rcalib,1,a
	Calc_Color_Threshold W_R_Thres_min, W_R_Thres_max, W_G_Thres_min, W_G_Thres_max, W_B_Thres_min, W_B_Thres_max
	flash_Reg tmp, 3, DUMP_REG, 0b00000001
    calibrate_for_green:
	movff rcalib,WREG
	bsf	DUMP_REG,1,a
	btfss	rcalib,0,a 
	bra $-2
	bcf	rcalib,1,a
	Calc_Color_Threshold G_R_Thres_min, G_R_Thres_max, G_G_Thres_min, G_G_Thres_max, G_B_Thres_min, G_B_Thres_max
	flash_Reg tmp, 3, DUMP_REG, 0b00000010
    calibrate_for_blue:
    	movff rcalib,WREG
	bsf	DUMP_REG,2,a
	btfss	rcalib,0,a 
	bra $-2
	bcf	rcalib,1,a
	Calc_Color_Threshold B_R_Thres_min, B_R_Thres_max, B_G_Thres_min, B_G_Thres_max, B_B_Thres_min, B_B_Thres_max
	flash_Reg tmp, 3, DUMP_REG, 0b00000100
    calibrate_for_red:
	movff rcalib,WREG
	bsf	DUMP_REG,3,a
	btfss	rcalib,0,a 
	bra $-2
	bcf	rcalib,1,a
	Calc_Color_Threshold R_R_Thres_min, R_R_Thres_max, R_G_Thres_min, R_G_Thres_max, R_B_Thres_min, R_B_Thres_max
	flash_Reg tmp, 3, DUMP_REG, 0b00001000
    calibrate_for_black:
	movff rcalib,WREG
	bsf	DUMP_REG,4,a
	btfss	rcalib,0,a 
	bra $-2
	bcf	rcalib,1,a
	Calc_Color_Threshold K_R_Thres_min, K_R_Thres_max, K_G_Thres_min, K_G_Thres_max, K_B_Thres_min, K_B_Thres_max
	flash_Reg tmp, 3, DUMP_REG, 0b00010000
    clrf DUMP_REG,a
    clrf rcalib,a
    return     
    
;========== Tests ==========
    
load_test_values:
    movlw   143
    movwf   R_R_Thres_min,a
    movlw   207
    movwf   R_R_Thres_max,a
    movlw   22
    movwf   R_G_Thres_min,a
    movlw   86
    movwf   R_G_Thres_max,a
    movlw   0
    movwf   R_B_Thres_min,a
    movlw   45
    movwf   R_B_Thres_max,a
    
    movlw   0
    movwf   G_R_Thres_min,a
    movlw   59
    movwf   G_R_Thres_max,a
    movlw   123
    movwf   G_G_Thres_min,a
    movlw   187
    movwf   G_G_Thres_max,a
    movlw   13
    movwf   G_B_Thres_min,a
    movlw   77
    movwf   G_B_Thres_max,a
    
    movlw   0
    movwf   B_R_Thres_min,a
    movlw   45
    movwf   B_R_Thres_max,a
    movlw   0
    movwf   B_G_Thres_min,a
    movlw   53
    movwf   B_G_Thres_max,a
    movlw   55
    movwf   B_B_Thres_min,a
    movlw   119
    movwf   B_B_Thres_max,a

    movlw   180
    movwf   W_R_Thres_min,a
    movlw   244
    movwf   W_R_Thres_max,a
    movlw   199
    movwf   W_G_Thres_min,a
    movlw   255
    movwf   W_G_Thres_max,a
    movlw   112
    movwf   W_B_Thres_min,a
    movlw   176
    movwf   W_B_Thres_max,a
   
    movlw   0
    movwf   K_R_Thres_min,a
    movlw   32
    movwf   K_R_Thres_max,a
    movlw   0
    movwf   K_G_Thres_min,a
    movlw   32
    movwf   K_G_Thres_max,a
    movlw   0
    movwf   K_B_Thres_min,a
    movlw   32
    movwf   K_B_Thres_max,a
    
    return