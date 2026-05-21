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
	
Race:
Sensor_LLI_Generate:
    clrf tmp,a
    
	movlw 0b01001101 ;AN19 a.k.a RC7, ADC on
	movwf ADCON0,0
	RGB_measure s1r, s1g, s1b ;Sensor 1

	movlw 0b01001001 ;AN18 a.k.a RC6, ADC on
	movwf ADCON0,0
	RGB_measure s2r, s2g, s2b ; Sensor 2

	movlw 0b01000101 ;AN17 a.k.a RC5, ADC on
	movwf ADCON0,0
	RGB_measure s3r, s3g, s3b ; Sensor 3

	movlw 0b01011101 ;AN23 a.k.a RD3, ADC on
	movwf ADCON0,0
	RGB_measure s4r, s4g, s4b ; Sensor 4

	movlw 0b01011001 ;AN22 a.k.a RD2, ADC on
	movwf ADCON0,0
	RGB_measure s5r, s5g, s5b ; Sensor 5
    
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
	bra $+4
	bra stop

    LLI_Entry:
	movff Sensor,prev_sensor
	btfsc Sensor,0
	bra right
	btfsc Sensor,4
	bra left
	btfsc Sensor,1
	bra slight_right
	btfsc Sensor,3
	bra slight_left
	btfsc Sensor,2
	bra straight
	bra Sensor_LLI_Generate

    right:		    
			;2_F ;1_F   ;2_B ;1_B
	set_motor_pwm   0x00,0xFA,  0x00,0x00
	bra Sensor_LLI_Generate
    
    slight_right:	  
			;1_F ;2_F   ;1_B ;2_B
	set_motor_pwm   0x00,0xAD,  0x00,0x00
    	bra Sensor_LLI_Generate
  
    left:		   
			;1_F ;2_F   ;1_B ;2_B
	set_motor_pwm	0xFA,0x00,  0x00,0x00
	bra Sensor_LLI_Generate

    
    slight_left:	    
			;1_F ;2_F   ;1_B ;2_B
	set_motor_pwm	0xAD,0x00,  0x00,0x00
	bra Sensor_LLI_Generate

   
    straight:	    
			;1_F ;2_F   ;1_B ;2_B
	set_motor_pwm	0xBA,0xBA,  0x00,0x00
        bra Sensor_LLI_Generate

 
    stop:		    
			;2_F ;1_F   ;2_B ;1_B
	set_motor_pwm   0x00,0x00,  0x00,0x00
	bra Sensor_LLI_Generate


