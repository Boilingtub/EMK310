set_tblptr macro lbl
    movlw highword(lbl)
    movwf TBLPTRU
    movlw high(lbl)
    movwf TBLPTRH
    movlw low(lbl)
    movwf TBLPTRL
endm
    
serial_test:
    movlw '>'
    call byte_tx
    movlw ' '
    call byte_tx
    movlw 0xD
    call byte_tx
    return
    
tx_FSR0:
    movf POSTINC0,0,0
    call byte_tx
    movlw 0xA
    cpfseq INDF0
    bra tx_FSR0
    return
   
byte_tx:
    movwf   TXREG2
poll_tx:
    btfss   TXSTA2,1
    bra    poll_tx
    return
    
match_rx:
    call echo_last_rx
    lfsr 0,rx_sto_addr
    movf POSTINC0,0,0
    movwf cmd  
    
    check_if_cmd_cyoc:
    movf cyoc,0,0
    cpfseq cmd
    bra check_if_mode_reg_cyoc
    movff cmd,mode_reg
    return
    
    check_if_mode_reg_cyoc:
    cpfseq mode_reg
    return
    movff cmd,mode_reg
    
    return
    
echo_last_rx:
    lfsr 0,rx_sto_addr
    movff rx_byte_count,tmp
    bra echo_loop_check
    echo_loop:
	movf POSTINC0,0,0
	call byte_tx
	echo_loop_check:
	    decfsz tmp
	    bra echo_loop
	    movlw 0x0A
	    call byte_tx
	    return
    
	
tx_startup_message:
    set_tblptr startup_message
    bra message_loop
   
tx_color_select_message:
    set_tblptr color_select_message
    bra message_loop
color_select_message:
    db "Please Select a color for the marv to follow",0xD
    db "R - selects Red Color",0xD
    db "G - selects Green Color",0xD
    db "B - selects Blue Color",0xD
    db "K - select Black Color",0xD,0xA
    
tx_invalid_color_message:
    set_tblptr invalid_color_message
    bra message_loop
invalid_color_message:
    db "Invalid color selected !",0xD,0xA
    
tx_calibrate_message:
    set_tblptr calibrate_message
    bra message_loop
calibrate_message:
    db "Marv now calibrating...",0xD
    db "Please put the marv on the instructed color "
    db "and press the touch sensor",0xD,0xA
    
tx_calibrate_calibrate_message:
    set_tblptr calibrate_calibrate_message
    bra message_loop 
calibrate_calibrate_message:
    db "calibrating for ",0xD,0xA
    
tx_calibrate_white_message:
    set_tblptr calibrate_white_message
    bra message_loop
calibrate_white_message:
    db "white",0xD,0xA,0x0
    
tx_calibrate_red_message:
    set_tblptr calibrate_red_message
    bra message_loop
calibrate_red_message:
    db "red",0xD,0xA,0x0
    
tx_calibrate_green_message:
    set_tblptr calibrate_green_message
    bra message_loop
calibrate_green_message:
    db "green",0xD,0xA,0x0
    
tx_calibrate_blue_message:
    set_tblptr calibrate_blue_message
    bra message_loop
calibrate_blue_message:
    db "blue",0xD,0xA
    
tx_calibrate_black_message:
    set_tblptr calibrate_black_message
    bra message_loop
calibrate_black_message:
    db "black",0xD,0xA,0x0
    
tx_success_message:
    set_tblptr success_message
    bra message_loop
success_message:
    db "success !",0xD,0xD,0xA
    
    
tx_simulate_message:
    set_tblptr simulate_message
    bra message_loop
simulate_message:
    db "Simulate Mode Started...",0xD,0xA
    
tx_simulate_forward_message:
    set_tblptr simulate_forward_message
    bra message_loop
simulate_forward_message:
    db "Forward !",0xD,0xA,0x0
    
tx_simulate_left_message:
    set_tblptr simulate_left_message
    bra message_loop
simulate_left_message:
    db "Left !",0xD,0xA
    
tx_simulate_right_message:
    set_tblptr simulate_right_message
    bra message_loop
simulate_right_message:
    db "Right !",0xD,0xA,0x0
    
tx_simulate_stop_message:
    set_tblptr simulate_stop_message
    bra message_loop
simulate_stop_message:
    db "Stop !",0xD,0xA
    
tx_attack_message:
    set_tblptr attack_message
    call message_loop
    movlw 0
    cpfseq nav_col
    bra $+4
    bra tx_calibrate_black_message
    
    movlw 1
    cpfseq nav_col
    bra $+4
    bra tx_calibrate_red_message
    
    movlw 2
    cpfseq nav_col
    bra $+4
    bra tx_calibrate_blue_message
    
    movlw 3
    cpfseq nav_col
    bra $+4
    bra tx_calibrate_green_message
attack_message:
    db "Attack ",0x0
    
message_loop:
    TBLRD*+
    movf TABLAT,0,0
    call byte_tx
    movlw 0xA
    cpfseq TABLAT
    bra $-12
    return
    





    
