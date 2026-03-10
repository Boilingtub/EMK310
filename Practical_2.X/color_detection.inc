check_navline macro sv, col_reg, rr, bit 
    movff nav_col,WREG
    determine_color sv,col_reg
    CPFSEQ col_reg,a
    bra $+4
    bsf rr,bit,a
endm

check_in_range macro r, min, max, next_label ;20 bytes PC
    movff min,WREG
    cpfslt r,a
    bra $+4
    bra $+10
    movff max,WREG
    cpfsgt r,a
    bra $+4
    bra next_label
endm
   
determine_color macro S,rr,rg,rb,rcol,done_label ; 322 bytes
    movlw 4
    movwf rcol,a
    ;check_white:
	check_in_range rr, S, S+1, $+44
	check_in_range rg, S+2, S+3, $+24
	check_in_range rb, S+4, S+5, $+4
	bra done_label
    ;check_green;
	decf rcol,a
	check_in_range rr, S+12, S+13, $+44
	check_in_range rg, S+14, S+15, $+24
	check_in_range rb, S+16, S+17, $+4
	bra done_label
   ;check_blue
   	decf rcol,a
	check_in_range rr, S+18, S+19, $+44
	check_in_range rg, S+20, S+21, $+24
	check_in_range rb, S+22, S+23, $+4
	bra done_label
   ;check_red
      	decf rcol,a
	check_in_range rr, S+6, S+7, $+44
	check_in_range rg, S+8, S+9, $+24
	check_in_range rb, S+10, S+11, $+4
	bra done_label
   ;check_black
      	decf rcol,a
	check_in_range rr, S+24, S+25, $+44
	check_in_range rg, S+26, S+27, $+24
	check_in_range rb, S+28, S+29, $+4
	bra done_label
endm
	
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
	determine_color S1_W_R_Thres_min,s1r,s1g,s1b,rcolor,col_det_done_S1
	col_det_done_S1:
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
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,3,a
	
	determine_color S5_W_R_Thres_min,s5r,s5g,s5b,rcolor,col_det_done_S5
	col_det_done_S5:
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
	clrf PORTA,a
	movff Sensor,PORTA
	goto left_logic
    return

    
Detect_LLI:
    btfsc PORTB,0,a
    bra $+6
    movlw 0
    bra do_Detect_LLI

   
    btfsc PORTB,1,a
    bra $+6
    movlw 1
    bra do_Detect_LLI

   
    btfsc PORTB,2,a
    bra $+6
    movlw 2
    bra do_Detect_LLI

   
    btfsc PORTB,3,a
    bra $+6
    movlw 3
    bra do_Detect_LLI
   
    movlw 2
    do_Detect_LLI:
	movwf nav_col,a
	call Sensor_LLI_Generate
	bra Detect_LLI
return

;========== TESTS ==========
	
color_detection_test:
    call read_Sensor5
    determine_color S5_W_R_Thres_min,s5r, s5g, s5b, rcolor, show_color
    bra color_detection_test
    show_color:
	call det_col_LED
	bra color_detection_test
    