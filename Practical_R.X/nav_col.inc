Set_Nav_col:
    clrf tmp,a
    
    btfss PORTB,6,a
    bra $+4
    bsf	tmp,0,b
   
    btfss PORTB,7,a
    bra $+4
    bsf tmp,1,a
    
    movlw 0
    cpfseq tmp,a
    bra $+8
    movlw 0b00000000
    movwf nav_col,a
    
    movlw 1
    cpfseq tmp,a
    bra $+8
    movlw 0b00000100
    movwf nav_col,a
    return
    
    movlw 2
    cpfseq tmp,a
    bra $+8
    movlw 0b00010000
    movwf nav_col,a
    return
    
    movlw 3
    cpfseq tmp,a
    bra $+8
    movlw 0b00001000
    movwf nav_col,a
    return
    
    movlw 0b00000000
    movwf nav_col,a
    return
   
    
get_SSD_from_nav_col:
    movlw 0
    cpfseq nav_col
    bra $+4
    retlw SSD_K
    
    movlw 1
    cpfseq nav_col
    bra $+4
    retlw SSD_R
    
    movlw 2
    cpfseq nav_col
    bra $+4
    retlw SSD_B
    
    movlw 3
    cpfseq nav_col
    bra $+4
    retlw SSD_G
    
    retlw SSD_0