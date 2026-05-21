check_in_range macro r, min, max, next_label ;20 bytes PC
    movff min,WREG
    cpfslt r,0
    bra $+4
    bra $+10
    movff max,WREG
    cpfsgt r,0
    bra $+4
    bra next_label
endm
   
determine_color macro S,rr,rg,rb,rcol,done_label ; 322 bytes
    movlb 0
    movlw 4
    movwf rcol,b
    ;check_white:
	check_in_range rr, S, S+1, $+44
	check_in_range rg, S+2, S+3, $+24
	check_in_range rb, S+4, S+5, $+4
	bra done_label
    ;check_green;
	decf rcol,b
	check_in_range rr, S+12, S+13, $+44
	check_in_range rg, S+14, S+15, $+24
	check_in_range rb, S+16, S+17, $+4
	bra done_label
   ;check_blue
   	decf rcol,b
	check_in_range rr, S+18, S+19, $+44
	check_in_range rg, S+20, S+21, $+24
	check_in_range rb, S+22, S+23, $+4
	bra done_label
   ;check_red
      	decf rcol,b
	check_in_range rr, S+6, S+7, $+44
	check_in_range rg, S+8, S+9, $+24
	check_in_range rb, S+10, S+11, $+4
	bra done_label
   ;check_black
      	decf rcol,b
	check_in_range rr, S+24, S+25, $+44
	check_in_range rg, S+26, S+27, $+24
	check_in_range rb, S+28, S+29, $+4
	bra done_label
    ;check_Invalid_color
	movlw 69
	movwf rcol,b
	bra done_label
endm
	
	
Sensor_LLI_Generate:
    ;clrf Sensor,a
    clrf tmp,a
    call read_Sensor_all
    get_LLI_color:
    
	determine_color S1_W_R_Thres_min,s1r,s1g,s1b,rcolor,col_det_done_S1
	col_det_done_S1:
    	bcf Sensor,0,a
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,0,a
	
	determine_color S2_W_R_Thres_min,s2r,s2g,s2b,rcolor,col_det_done_S2
	col_det_done_S2:
	bcf Sensor,1,a
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,1,a

	determine_color S3_W_R_Thres_min,s3r,s3g,s3b,rcolor,col_det_done_S3
	col_det_done_S3:
	bcf Sensor,2,a
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,2,a
	
	determine_color S4_W_R_Thres_min,s4r,s4g,s4b,rcolor,col_det_done_S4
	col_det_done_S4:
	bcf Sensor,3,a
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	cpfseq rcolor,a
	bra $+4
	bsf Sensor,3,a
	
	determine_color S5_W_R_Thres_min,s5r,s5g,s5b,rcolor,col_det_done_S5
	col_det_done_S5:
	bcf Sensor,4,a
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
	call stop
	bra $+4
	call LLI_Entry
	
    return

    
Check_Nav_Col:
    call Check_Nav_Select
    movwf nav_col,a
    return

Set_Nav_col:
    clrf tmp,a
    
    btfss PORTB,6,a
    bra $+4
    bsf	tmp,0,b
   
    btfss PORTB,7,a
    bra $+4
    bsf tmp,1,a
    
    movlw 0
    cpfseq tmp,a
    bra $+8
    movlw 0b00000000
    movwf nav_col,a
    
    movlw 1
    cpfseq tmp,a
    bra $+8
    movlw 0b00000100
    movwf nav_col,a
    return
    
    movlw 2
    cpfseq tmp,a
    bra $+8
    movlw 0b00010000
    movwf nav_col,a
    return
    
    movlw 3
    cpfseq tmp,a
    bra $+8
    movlw 0b00001000
    movwf nav_col,a
    return
    
    movlw 0b00000000
    movwf nav_col,a
    return
   
    
get_SSD_from_nav_col:
    movlw 0
    cpfseq nav_col
    bra $+4
    retlw SSD_K
    
    movlw 1
    cpfseq nav_col
    bra $+4
    retlw SSD_R
    
    movlw 2
    cpfseq nav_col
    bra $+4
    retlw SSD_B
    
    movlw 3
    cpfseq nav_col
    bra $+4
    retlw SSD_G
    
    retlw SSD_0

;========== TESTS ==========
	
color_detection_test:
    call read_Sensor1
    determine_color S1_W_R_Thres_min,s1r, s1g, s1b, rcolor, show_color
    bra color_detection_test
    show_color:
	call det_col_LED
	bra color_detection_test
	
    