ISRH:
    
   
    btfsc RC2IF
    bra RC_ISR

    
    bra ISRH_done
    
    RC_ISR:
	;bcf RC2IF
	btfss rx_done,0
	bra RC_read
	clrf rx_byte_count
	lfsr 0,rx_sto_addr
	clrf rx_done
	RC_read:
	    movf RCREG2,0,0
	    movwf POSTINC0
	    incf rx_byte_count
	
	    movf RCSTA2,0,0
	    
	    movlw 0x0A
	    cpfseq RCREG2
	    bra $+4
	    bra RC_READ_DONE
	    
	    movlw 0xFE
	    cpfseq rx_byte_count
	    bra $+4
	    bra RC_READ_DONE
	
	    bra ISRH_done
	
	RC_READ_DONE:
	    bsf rx_done,0
	    call match_rx
	    bra ISRH_done
	    	    
	ISR_NAV_COL:
	    movf PORTB,0,0
	    bcf RBIF
	    call Check_Nav_Col
	    bra ISRH_done
	    
    ISRH_done:
	retfie


