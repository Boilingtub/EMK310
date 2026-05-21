p3_color_select:
    movlw SSD_1
    movwf PORTA
    
    call tx_color_select_message
    
    movlw 'C'
    cpfseq cmd,0
    bra $+4
    bra $-6
    
    movlw 'R'
    cpfseq cmd
    bra p3_color_select_G
    movlw 1
    movwf nav_col
    bra p3_color_select_done
    
    p3_color_select_G:
	movlw 'G'
	cpfseq cmd
	bra p3_color_select_B
	movlw 3
	movwf nav_col
	bra p3_color_select_done

    p3_color_select_B:
	movlw 'B'
	cpfseq cmd
	bra p3_color_select_K
	movlw 3
	movwf nav_col
	bra p3_color_select_done
    
    p3_color_select_K:
	movlw 'K'
	cpfseq cmd
	bra p3_color_select_invalid
	movlw 4
	movwf nav_col
	bra p3_color_select_done
	
    p3_color_select_invalid:
	movlw 'U'
	movwf cmd
	call tx_invalid_color_message
	bra p3_color_select
	
    p3_color_select_done:
	bsf rflags,NAV_COL
	movff cyoc,mode_reg
	goto p3_loop

	
p3_calibrate:
    movlw SSD_2
    movwf PORTA
    call calibrate
    movff cyoc,mode_reg
    goto p3_loop
    
p3_attack:
    movlw SSD_3
    movwf PORTA
    
    call tx_attack_message
 
    race_wait_for_touch:
    	movlw 'A'
	cpfseq mode_reg
	goto p3_loop
	
	
	call read_touch
	tstfsz cap_reg,a
	bra $+4
	goto race_wait_for_touch
	
	call get_SSD_from_nav_col
	movwf PORTD
    
	movlw 'A'
	cpfseq mode_reg
	goto p3_loop
	
	
    p3_race_mode:
	btfss rflags,CALIBRATED
	bra $+4
	bra p3_attack_loop
	
	call calibrate
	bra p3_race_mode

    p3_attack_loop:
        btfss rflags,NAV_COL
	call Check_Nav_Col
	bra p3_attack_loop_go

	p3_attack_loop_go:
	    call Sensor_LLI_Generate
	    movlw 'A'
	    cpfseq mode_reg
	    goto p3_loop
	    goto p3_attack_loop
    
	    
	   
wait_for_touch_to_race:
    call read_touch
	
    movlw 'A'
    cpfseq mode_reg
    goto p3_loop
	
    tstfsz cap_reg,a
    bra p3_race_mode
    bra wait_for_touch_to_race
    
p3_simulate:
    movlw SSD_4
    movwf PORTA
    call tx_simulate_message
    p3_simulate_loop:
	movlw 'S'
	cpfseq cmd
	bra $+4
	bra p3_simulate_sensor
    
	movlw 'F'
	cpfseq cmd
	bra $+4
	bra p3_simulate_forward
    
	movlw 'L'
	cpfseq cmd
	bra $+4
	bra p3_simulate_left
    
	movlw 'R'
	cpfseq cmd
	bra $+4
	bra p3_simulate_right
    
	bra p3_simulate_loop
      
	movlw 'S'
	cpfseq mode_reg
	goto p3_loop
	bra p3_simulate_loop
    
p3_hotload:
    movlw SSD_5
    movwf PORTA
    call tx_hotload_message
    
    call EEPROM_slogan_message
    lfsr 0,i2c_sto_addr
    call tx_FSR0
    
    bcf rflags,RX_STRING
    btfss rflags,RX_STRING
    bra $-2
    
    
    lfsr 0,rx_sto_addr
    call write_lfsr_to_eeprom
    
    call EEPROM_slogan_message
    lfsr 0,i2c_sto_addr
    call tx_FSR0
    
    movff cyoc,mode_reg
    goto p3_loop
	
p3_cyoc:
    movlw SSD_6
    movwf PORTA,0
    call EEPROM_menu_message
    lfsr 0,i2c_sto_addr
    call tx_FSR0 
    p3_cyoc_loop:
    movf cyoc,0,0
    cpfseq mode_reg
    goto p3_loop
    bra p3_cyoc_loop


