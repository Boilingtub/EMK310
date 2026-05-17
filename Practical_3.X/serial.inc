serial_test:
    movlw '>'
    call byte_tx
    movlw ' '
    call byte_tx
    movlw 0xD
    call byte_tx
    return
    
byte_tx:
    movwf   TXREG2
poll_tx:
    btfss   TXSTA2,1
    bra    poll_tx
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
	return
    
	
tx_startup_message:
    movlw 0x00
    movwf TBLPTRU
    movlw 0x50
    movwf TBLPTRH
    movlw 0x00
    movwf TBLPTRL
    
    startup_message_loop:
	TBLRD*+
	movf TABLAT,0,0
	call byte_tx
	movwf 0xD
	cpfseq TABLAT
	bra startup_message_loop
	return
    
org 0x5000
startup_message:
    db "St",0xD
    db "Jy kan maar glo Jessie baby daar's geen ander een",0xD