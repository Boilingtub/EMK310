check_navline macro sv, col_reg, rr, bit 
    movff nav_col,WREG
    determine_color sv,col_reg
    CPFSEQ col_reg,a
    bra $+4
    bsf rr,bit,a
endm

check_in_range macro r, min, max, false_label ;22 bytes PC
    movff min,WREG
    cpfslt r,a
    bra $+4
    bra $+10
    movff max,WREG
    cpfsgt s1r,a
    bra $+4
    bra false_label
endm
   
determine_color macro rr,rg,rb,rcol
    movlw 4
    movwf rcol,a
    ;check_white:
	check_in_range rr, W_R_Thres_min, W_R_Thres_max, $+44
	check_in_range rg, W_G_Thres_min, W_G_Thres_max, $+24
	check_in_range rb, W_R_Thres_min, W_R_Thres_max, $+4
	bra $+4
	bra $+66
    ;check_green;
	decf rcol,a
	check_in_range rr, G_R_Thres_min, G_R_Thres_max, $+44
	check_in_range rg, G_G_Thres_min, G_G_Thres_max, $+24
	check_in_range rb, G_R_Thres_min, G_R_Thres_max, $+4
	bra $+4
	bra $+66
   ;check_blue
   	decf rcol,a
	check_in_range rr, B_R_Thres_min, B_R_Thres_max, $+44
	check_in_range rg, B_G_Thres_min, B_G_Thres_max, $+24
	check_in_range rb, B_R_Thres_min, B_R_Thres_max, $+4
	bra $+4
	bra $+66
   ;check_red
      	decf rcol,a
	check_in_range rr, R_R_Thres_min, R_R_Thres_max, $+44
	check_in_range rg, R_G_Thres_min, R_G_Thres_max, $+24
	check_in_range rb, R_R_Thres_min, R_R_Thres_max, $+4
	bra $+4
	bra $+66
   ;check_black
      	decf rcol,a
	check_in_range rr, K_R_Thres_min, K_R_Thres_max, $+44
	check_in_range rg, K_G_Thres_min, K_G_Thres_max, $+24
	check_in_range rb, K_R_Thres_min, K_R_Thres_max, $+4
	bra $+4
	bra $+6
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
	determine_color s1r,s1g,s1b,rcolor
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,0,a
	
	determine_color s2r,s2g,s2b,rcolor
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,1,a

	determine_color s3r,s3g,s3b,rcolor
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,2,a
	
	determine_color s4r,s4g,s4b,rcolor
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	CPFSEQ rcolor,a
	bra $+4
	bsf Sensor,3,a
	
	determine_color s5r,s5g,s5b,rcolor
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
   
    do_Detect_LLI:
	movwf nav_col,a
	call Sensor_LLI_Generate
	bra Detect_LLI
