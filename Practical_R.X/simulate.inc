p3_simulate_sensor:
    lfsr 0,0x400
    
    call read_Sensor_all
    
    determine_color S1_W_R_Thres_min,s1r,s1g,s1b,rcolor,simulate_col_det_done_S1
    simulate_col_det_done_S1:
    call p3_simulate_det_col
    determine_color S2_W_R_Thres_min,s2r,s2g,s2b,rcolor,simulate_col_det_done_S2
    simulate_col_det_done_S2:
    call p3_simulate_det_col
    determine_color S3_W_R_Thres_min,s3r,s3g,s3b,rcolor,simulate_col_det_done_S3
    simulate_col_det_done_S3:
    call p3_simulate_det_col
    determine_color S4_W_R_Thres_min,s4r,s4g,s4b,rcolor,simulate_col_det_done_S4
    simulate_col_det_done_S4:
    call p3_simulate_det_col
    determine_color S5_W_R_Thres_min,s5r,s5g,s5b,rcolor,simulate_col_det_done_S5
    simulate_col_det_done_S5:
    call p3_simulate_det_col

    
    ;movlw 0
    ;movwf rcount
    ;simulate_compare_sensor:
    ;	movf rcount,0,0
    ;	    lfsr 0,0x400
    ;	    movff PLUSW0,tmp
    ;	    lfsr 0,0x405
    ;	    movf PLUSW0,0,0
    ;	cpfseq tmp
    ;	bra tx_simulate_sensor
    ;	incf rcount
    ;	movlw 5
    ;	cpfseq rcount
    ;	bra simulate_compare_sensor
    ;	bra simulate_sensor_done


    tx_simulate_sensor:
        movlw 5
	movwf rcount
	lfsr 0,0x400
	tx_simulate_sensor_loop:
	    movf INDF0,0,0
	    call byte_tx
	    decfsz rcount
	    bra tx_simulate_sensor_loop
	    movlw 0xD
	    call byte_tx

    ;simulate_sensor_done:
    ;	movlw 4
    ;	simulate_sensor_done_loop:
    ;	    lfsr 0,0x400
    ;	    movff PLUSW0,tmp
    ;	    lfsr 0,0x405
    ;	    movff tmp,PLUSW0
    ;	    decfsz WREG
    ;	    bra simulate_sensor_done_loop
	    
    bra p3_simulate_loop

p3_simulate_det_col:
    movlw 0
    cpfseq rcolor
    bra $+10
    movlw 'K'
    movwf POSTINC0
    return
    
    movlw 1
    cpfseq rcolor
    bra $+10
    movlw 'R'
    movwf POSTINC0
    return
    
    movlw 2
    cpfseq rcolor
    bra $+10
    movlw 'B'
    movwf POSTINC0
    call byte_tx
    return
    
    movlw 3
    cpfseq rcolor
    bra $+10
    movlw 'G'
    movwf POSTINC0
    call byte_tx
    return
    
    movlw 4
    cpfseq rcolor
    bra $+10
    movlw 'W'
    movwf POSTINC0
    call byte_tx
    return
    
    movlw 'U'
    movwf POSTINC0
    return
    
    
    
    
p3_simulate_forward:
    movlw 0xF
    tstfsz CCPR2L
    bra simulate_forward_stop
    bra simulate_forward_straight
    simulate_forward_stop:
        call tx_simulate_stop_message
	call stop
	bra p3_simulate_continue
    simulate_forward_straight:
        call tx_simulate_forward_message
	call straight
	bra p3_simulate_continue

    
p3_simulate_left:
    movlb 0xF
    tstfsz CCPR2L
    bra simulate_left_stop
    bra simulate_left_left
    simulate_left_stop:
        call tx_simulate_stop_message
	call stop
	bra p3_simulate_continue
    simulate_left_left:
        call tx_simulate_left_message
	call left
	bra p3_simulate_continue

    
p3_simulate_right:
    movlb 0xF
    tstfsz CCPR3L
    bra simulate_right_stop
    bra simulate_right_right
    simulate_right_stop:
        call tx_simulate_stop_message
	call stop
	bra p3_simulate_continue
    simulate_right_right:
        call tx_simulate_right_message
	call right
	bra p3_simulate_continue
    
	
p3_simulate_continue:
    movlb 0x0
    movlw 'S'
    movwf cmd
    bra p3_simulate_loop
    