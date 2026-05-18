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
	movlw 'C'
	movwf cmd
	call tx_invalid_color_message
	bra p3_color_select
	
    p3_color_select_done:
	movff cyoc,mode_reg
	bra p3_loop

	
p3_calibrate:
    movlw SSD_2
    movwf PORTA
    call calibrate
    movff cyoc,mode_reg
    bra p3_loop
    
p3_attack:
    movlw SSD_3
    movwf PORTA
    
    
    
    movlw 'A'
    cpfseq mode_reg
    bra p3_loop
    bra p3_attack
    
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
	bra p3_loop
	bra p3_simulate_loop
    
p3_hotload:
    movlw SSD_5
    movwf PORTA
    
    movlw 'H'
    cpfseq mode_reg
    bra p3_loop
    bra p3_hotload
	
p3_cyoc:
    call EEPROM_menu_message
    call tx_FSR0 
    p3_cyoc_loop:
    movf cyoc,0,0
    cpfseq mode_reg
    bra p3_loop
    bra p3_cyoc_loop


