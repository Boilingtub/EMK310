Set_Nav_col:
    clrf tmp,a
    btfsc PORTD,5
    bra go_red
    btfsc PORTD,6
    bra go_blue
    btfsc PORTD,7
    bra go_green
    bra go_red
    go_red:
	movlw 1
	movwf nav_col
	return
    go_blue:
	movlw 2
	movwf nav_col
	return
    go_green:
	movlw 3
	movwf nav_col
	return
   
set_SSD_from_nav_col:
    movlw 0
    cpfseq nav_col
    bra $+8
    movlw SSD_K
    movwf PORTA
    return
    
    movlw 1
    cpfseq nav_col
    bra $+8
    movlw SSD_R
    movwf PORTA
    return
    
    movlw 2
    cpfseq nav_col
    bra $+8
    movlw SSD_B
    movwf PORTA
    return
    
    movlw 3
    cpfseq nav_col
    bra $+8
    movlw SSD_G
    movwf PORTA
    return
    
    movlw SSD_0
    movwf PORTA
    return