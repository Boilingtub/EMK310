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
    cpfslt v,b
    bra $+8
    movlw 0
    movwf rr,b
    bra $+10
    movff v,rr
    movlw Thres_var
    subwf rr,b
endm
 
calc_threshold_max macro margin,v,rr
    movlw 255-margin
    cpfsgt v,b
    bra $+8
    movlw 255
    movwf rr,b
    bra $+10
    movff v,rr
    movlw margin
    addwf rr,b
endm
    
Calc_Color_Threshold macro margin,rr,rg,rb,thres
    calc_threshold_min Thres_var, rr, thres
    calc_threshold_max Thres_var, rr, thres+1
    calc_threshold_min Thres_var, rg, thres+2
    calc_threshold_max Thres_var, rg, thres+3
    calc_threshold_min Thres_var, rb, thres+4
    calc_threshold_max Thres_var, rb, thres+5
endm

calibrate:
calibrate_start:
    clrf SSD,a
    calibrate_for_white:
	movlw SSD_W
	movwf SSD,a
	call wait_for_touch
	call read_Sensor_all
	Calc_Color_Threshold Thres_var,s1r,s1g,s1b, S1_W_R_Thres_min
	Calc_Color_Threshold Thres_var,s2r,s2g,s2b, S2_W_R_Thres_min
	Calc_Color_Threshold Thres_var,s3r,s3g,s3b, S3_W_R_Thres_min
	Calc_Color_Threshold Thres_var,s4r,s4g,s4b, S4_W_R_Thres_min
	Calc_Color_Threshold Thres_var,s5r,s5g,s5b, S5_W_R_Thres_min
	flash_Reg tmp, 3, SSD, SSD_W
    calibrate_for_green:
	movlw SSD_G
	movwf SSD,a 
	call wait_for_touch
	call read_Sensor_all
	Calc_Color_Threshold Thres_var,s1r,s1g,s1b, S1_G_R_Thres_min
	Calc_Color_Threshold Thres_var,s2r,s2g,s2b, S2_G_R_Thres_min
	Calc_Color_Threshold Thres_var,s3r,s3g,s3b, S3_G_R_Thres_min
	Calc_Color_Threshold Thres_var,s4r,s4g,s4b, S4_G_R_Thres_min
	Calc_Color_Threshold Thres_var,s5r,s5g,s5b, S5_G_R_Thres_min
	flash_Reg tmp, 3, SSD, SSD_G
    calibrate_for_blue:
	movlw SSD_B
	movwf SSD,a
	call wait_for_touch
	call read_Sensor_all
	Calc_Color_Threshold Thres_var,s1r,s1g,s1b, S1_B_R_Thres_min
	Calc_Color_Threshold Thres_var,s2r,s2g,s2b, S2_B_R_Thres_min
	Calc_Color_Threshold Thres_var,s3r,s3g,s3b, S3_B_R_Thres_min
	Calc_Color_Threshold Thres_var,s4r,s4g,s4b, S4_B_R_Thres_min
	Calc_Color_Threshold Thres_var,s5r,s5g,s5b, S5_B_R_Thres_min
	flash_Reg tmp, 3, SSD, SSD_B
    calibrate_for_black:
	movlw SSD_K
	movwf SSD,a
	call wait_for_touch
	call read_Sensor_all
	Calc_Color_Threshold Black_Thres_var,s1r,s1g,s1b, S1_K_R_Thres_min
	Calc_Color_Threshold Black_Thres_var,s2r,s2g,s2b, S2_K_R_Thres_min
	Calc_Color_Threshold Black_Thres_var,s3r,s3g,s3b, S3_K_R_Thres_min
	Calc_Color_Threshold Black_Thres_var,s4r,s4g,s4b, S4_K_R_Thres_min
	Calc_Color_Threshold Black_Thres_var,s5r,s5g,s5b, S5_K_R_Thres_min
	flash_Reg tmp, 3, SSD, SSD_K
    calibrate_for_red:
	movlw SSD_R
	movwf SSD,a
	call wait_for_touch
	call read_Sensor_all
	Calc_Color_Threshold Thres_var,s1r,s1g,s1b, S1_R_R_Thres_min
	Calc_Color_Threshold Thres_var,s2r,s2g,s2b, S2_R_R_Thres_min
	Calc_Color_Threshold Thres_var,s3r,s3g,s3b, S3_R_R_Thres_min
	Calc_Color_Threshold Thres_var,s4r,s4g,s4b, S4_R_R_Thres_min
	Calc_Color_Threshold Thres_var,s5r,s5g,s5b, S5_R_R_Thres_min
	flash_Reg tmp, 3, SSD, SSD_R
    clrf SSD,a
    return     
    
read_Sensor_all:
	movlw 0b01001101 ;AN19 a.k.a RC7, ADC on
	movwf ADCON0,0 
	RGB_measure s1r, s1g, s1b ; Sensor 1

	movlw 0b01001001 ;AN18 a.k.a RC6, ADC on
	movwf ADCON0,0
	RGB_measure s2r, s2g, s2b ; Sensor 2

	movlw 0b01000101 ;AN17 a.k.a RC5, ADC on
	movwf ADCON0,0
	RGB_measure s3r, s3g, s3b ;Sensor 3

	movlw 0b01011101 ;AN23 a.k.a RD3, ADC on
	movwf ADCON0,0
	RGB_measure s4r, s4g, s4b ;Sensor 4

	movlw 0b01011001 ;AN22 a.k.a RD2, ADC on
	movwf ADCON0,0
	RGB_measure s5r, s5g, s5b ;Sensor 5
return