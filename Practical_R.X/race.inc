wait_timer macro th,tl ;Wait_time = 0xFFFF - (OxFFFF/(Hz*2))
    ;YYXX = 65535 - (time required)/((4/fosc)*prescaler)
    ;YYXX = 44723 -> 0XAEB3 in hex	
    movlw 0x83
    movwf T0CON, c ;c specifies that it is the access bank
    ;preloading the timer
    movlw th	;Timer High bits
    movwf TMR0H, c
    movlw tl	;Timer Low Bits
    movwf TMR0L, c	
    ;clearing the timer overflow flag and turning on the timer
    bcf INTCON, 2, c
    bsf T0CON, 7, c
    
    btfss INTCON, 2, c
    bra $-4		
    bcf T0CON, 7, c ; timer is turned off
endm

ADC_measure macro r1
    ;wait_timer ADCAQTH,ADCAQTL  
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
  
set_motor_pwm macro ccp2, ccp3, ccp4, ccp5
    movlb 0xF
    movlw   ccp2    ; Duty Cycle for CCP2 (RB3) but3 off for setup
    movwf   CCPR2L,1 ;M1 F
    
    movlw   ccp3  ; Duty Cycle for CCP3 (RE0) but off for setup
    movwf   CCPR3L,1 ;M2 F
    
    movlw   ccp4  ; Duty Cycle for CCP1 (RD1) off for setup
    movwf   CCPR4L,1 ;M1 B
    
    movlw   ccp5  ; Duty Cycle for CCP5 (RE2) off for setup
    movwf   CCPR5L,1 ;M2 B
    movlb 0x0
endm
    
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
	
    
sub_reg macro reg,step
    movlw step
    cpfslt reg
    bra $+6
    clrf reg
    bra $+4
    subwf reg
endm
    
add_reg macro reg,step,max
    movlw max-step
    cpfsgt reg
    bra $+8
    movlw max
    movwf reg
    bra $+6
    movlw step
    addwf reg
endm
	
Race:
    clrf m1f
    clrf m2f
    clrf m1b
    clrf m2b
    goto straight
    Sensor_LLI_Generate:
        movlb 0xF 			;2_F ;1_F   ;2_B ;1_B
	; Duty Cycle for CCP2 (RB3) but3 off for setup
	movff   m2f,CCPR2L ;M2 F
    
	; Duty Cycle for CCP3 (RE0) but off for setup
	movff   m1f,CCPR3L ;M1 F
    
	; Duty Cycle for CCP1 (RD1) off for setup
	movff   m2b,CCPR4L ;M2 B
    
	; Duty Cycle for CCP5 (RE2) off for setup
	movff   m1b,CCPR5L ;M1 B
	movlb 0x0

	clrf tmp
		
	movlw 0b01011101 ;AN23 a.k.a RD3, ADC on
	movwf ADCON0,0
	RGB_measure s4r, s4g, s4b ; Sensor 4
	determine_color S4_W_R_Thres_min,s4r,s4g,s4b,rcolor,col_det_done_S4
	col_det_done_S4:
	movlw 0
	cpfseq rcolor,a
	bra $+4
	incf tmp,a
	;=========================
	movff nav_col,WREG
	cpfseq rcolor,a
	bra $+4
	bra slight_left
	
	movlw 0b01001001 ;AN18 a.k.a RC6, ADC on
	movwf ADCON0,0
	RGB_measure s2r, s2g, s2b ; Sensor 2
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
	bra slight_right
	
	movlw 0b01001101 ;AN19 a.k.a RC7, ADC on
	movwf ADCON0,0
	RGB_measure s1r, s1g, s1b ;Sensor 1
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
	bra right
	
	movlw 0b01011001 ;AN22 a.k.a RD2, ADC on
	movwf ADCON0,0
	RGB_measure s5r, s5g, s5b ; Sensor 5
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
	bra left
	
	movlw 0b01000101 ;AN17 a.k.a RC5, ADC on
	movwf ADCON0,0
	RGB_measure s3r, s3g, s3b ; Sensor 3
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
	bra straight
		
	movlw 5
	cpfseq tmp,a
	bra $+4
	bra stop
	
	btfsc prev_sensor,4
	bra right
	btfsc prev_sensor,3
	bra slight_right
	btfsc prev_sensor,2
	bra straight
	btfsc prev_sensor,1
	bra slight_left
	btfsc prev_sensor,0
	bra left
	
	goto Sensor_LLI_Generate

    right:		    
			;2_F ;1_F   ;2_B ;1_B
	;set_motor_pwm   0x00, 254,   254,0x00
	clrf prev_sensor
	bsf prev_sensor,4
	clrf stop_set
	movlw 0
	cpfsgt m1b
	bra right_inc_m1f
	sub_reg m1b, MOTOR_STEP_BIG
	bra right_do_rest
	right_inc_m1f:
	add_reg m1f, MOTOR_STEP_BIG, MOTOR_TURN_FWD
	
	right_do_rest:
	movlw 0
	cpfsgt m2f
	bra right_inc_m2b
	sub_reg m2f, MOTOR_STEP_BIG
	bra right_finish
	right_inc_m2b:
	add_reg m2b, MOTOR_STEP_BIG, MOTOR_TURN_REV
	
	right_finish:
	goto Sensor_LLI_Generate
    
    slight_right:	  
			;2_F ;1_F   ;2_B ;1_B
	;set_motor_pwm   127,254,  0x00,0x00
	clrf prev_sensor
	bsf prev_sensor,3
	clrf stop_set
	
	movlw 0
	cpfsgt m2b
	bra slight_right_inc_m2f
	sub_reg m2b, MOTOR_STEP_SMALL
	bra slight_right_do_rest
	slight_right_inc_m2f:	
	movlw 127
	cpfslt m2f
	bra slight_right_m2f_sub
	add_reg m2f, MOTOR_STEP_SMALL, MOTOR_SLIGHT_FWD
	bra slight_right_do_rest
	slight_right_m2f_sub:
	sub_reg m2f, MOTOR_STEP_SMALL
	
	slight_right_do_rest:
	movlw 0
	cpfsgt m1b
	bra slight_right_inc_m1f
	sub_reg m1b, MOTOR_STEP_SMALL
	bra slight_right_finish
	slight_right_inc_m1f:
	add_reg m1f, MOTOR_STEP_SMALL, MOTOR_SLIGHT_TURN
	
	slight_right_finish:
    	goto Sensor_LLI_Generate
  
    left:		   
			;2_F ;1_F   ;2_B ;1_B
	;set_motor_pwm	 254,0x00,  0x00,254
	clrf prev_sensor
	bsf prev_sensor,0
	clrf stop_set
	movlw 0
	cpfsgt m1f
	bra left_inc_m1b
	sub_reg m1f, MOTOR_STEP_BIG
	bra right_do_rest
	left_inc_m1b:
	add_reg m1b, MOTOR_STEP_BIG, MOTOR_TURN_FWD
	
	left_do_rest:
	movlw 0
	cpfsgt m2b
	bra left_inc_m2f
	sub_reg m2b, MOTOR_STEP_BIG
	bra left_finish
	left_inc_m2f:
	add_reg m2f, MOTOR_STEP_BIG, MOTOR_TURN_REV
	
	left_finish:
	goto Sensor_LLI_Generate

    
    slight_left:	    
			;2_F ;1_F   ;2_B ;1_B
	;set_motor_pwm	254, 127,  0x00,0x00
	clrf stop_set
	clrf prev_sensor
	bsf prev_sensor,1
	
	movlw 0
	cpfsgt m1b
	bra slight_left_inc_m1f
	sub_reg m1b, MOTOR_STEP_SMALL
	bra slight_left_do_rest
	slight_left_inc_m1f:	
	movlw 127
	cpfslt m1f
	bra slight_left_m1f_sub
	add_reg m1f, MOTOR_STEP_SMALL, MOTOR_SLIGHT_FWD
	bra slight_left_do_rest
	slight_left_m1f_sub:
	sub_reg m1f, MOTOR_STEP_SMALL
	
	slight_left_do_rest:
	movlw 0
	cpfsgt m2b
	bra slight_left_inc_m2f
	sub_reg m2b, MOTOR_STEP_SMALL
	bra slight_left_finish
	slight_left_inc_m2f:
	add_reg m2f, MOTOR_STEP_SMALL, MOTOR_SLIGHT_TURN
	
	slight_left_finish:	
	goto Sensor_LLI_Generate

   
    straight:	    
			;2_F ;1_F   ;2_B ;1_B
	;set_motor_pwm	254, 254,  0x00,0x00
	clrf prev_sensor
	bsf prev_sensor,2
	clrf stop_set
	movlw 0
	cpfsgt m1b
	bra straight_inc_m1f
	sub_reg m1b, MOTOR_STEP_BIG
	bra straight_do_rest
	straight_inc_m1f:
	add_reg m1f, MOTOR_STEP_BIG, MOTOR_STRAIGHT
	
        straight_do_rest:
	movlw 0
	cpfsgt m2b
	bra left_inc_m2f
	sub_reg m2b, MOTOR_STEP_BIG
	bra straight_finish
	straight_inc_m2f:
	add_reg m2f, MOTOR_STEP_BIG, MOTOR_STRAIGHT
	
	straight_finish:
        goto Sensor_LLI_Generate

    stop:		    
			;2_F ;1_F   ;2_B ;1_B
	;set_motor_pwm   0x00,0x00,  0x00,0x00
	movlw 5
	cpfseq stop_set
	bra $+4
	bra stop_do
	incf stop_set
	goto Sensor_LLI_Generate
	stop_do:
	sub_reg m2f, MOTOR_STEP_BIG
	sub_reg m1f, MOTOR_STEP_BIG
	sub_reg m2b, MOTOR_STEP_BIG 
	sub_reg m1b, MOTOR_STEP_BIG
	goto Sensor_LLI_Generate