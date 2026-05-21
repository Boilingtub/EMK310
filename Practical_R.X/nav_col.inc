Set_Nav_col:
    clrf tmp,a
    
    btfss PORTB,6,a
    bra $+4
    bsf	tmp,0
   
    btfss PORTB,7,a
    bra $+4
    bsf tmp,1
    
    movlw 0
    cpfseq tmp,a
    bra $+6
    movwf nav_col,a
    return
    
    movlw 1
    cpfseq tmp,a
    bra $+6
    movwf nav_col,a
    return
    
    movlw 2
    cpfseq tmp,a
    bra $+6
    movwf nav_col,a
    return
    
    movlw 3
    cpfseq tmp,a
    bra $+6
    movwf nav_col,a
    return
    
    movwf nav_col,a
    return
   
    
set_SSD_from_nav_col:
    movlw 0
    cpfseq nav_col
    bra $+4
    movwf PORTA
    return
    
    movlw 1
    cpfseq nav_col
    bra $+4
    movwf PORTA
    return
    
    movlw 2
    cpfseq nav_col
    bra $+4
    movwf PORTA
    return
    
    movlw 3
    cpfseq nav_col
    bra $+4
    movwf PORTA
    return
    
    movwf PORTA
    return