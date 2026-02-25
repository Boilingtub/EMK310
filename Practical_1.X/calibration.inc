toggle_RGB macro out_port, addr_port, rgb_val
    movff   addr_port,  WREG, a ;Move Port B into Working register 
    andlw   rgb_val
    tstfsz  WREG
    bra	    $+4
    bra	    $+6
    movlw   0b00000000
    bra	    $+4
    movlw   rgb_val
    movwf   addr_port,a
    movwf   out_port,a
endm

take_RGB_measurement macro reg_col
    movlw 0b00000001 ;AN0, ADC on
    movwf ADCON0,a
    bsf	  ADCON0,1,1 ;Start conversion
    btfsc ADCON0,1 ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, ADC_RH,a 	; Result is complete - store 2 MSbits in
    movff ADRESL, ADC_RL,a	; RESULTHI and 8 LSbits in RESULTLO
    movff ADRESL, reg_col,a
endm

calibrate_RGB macro reg_col, count_addr, count_val, out_port, addr_port, rgb_val
    movlw count_val
    movwf count_addr,a
    toggle_RGB out_port, addr_port, rgb_val   ;toggle_RGB is	22 bytes
    take_RGB_measurement reg_col	      ;Measure is	22 bytes
    ;call ADC_measure
    wait_timer 0xD5,0x55		      ;wait_timer is    22 bytes	
    decfsz  count_addr,f		      ;decfsz is	2 bytes
    bra	    $-68			      ;put PC		62 bytes back
endm

RGB_calibrate_all:
    calibrate_RGB reg_R, flash_counter, 6, PORTB, PORTB_val, RGB_R
    calibrate_RGB reg_G, flash_counter, 6, PORTB, PORTB_val, RGB_G
    calibrate_RGB reg_B, flash_counter, 6, PORTB, PORTB_val, RGB_B
    calibrate_RGB reg_W, flash_counter, 6, PORTB, PORTB_val, RGB_W
    return
        
RGB_calibrate_test:
    bsf	    PORTC,3,a
    bsf	    PORTC,4,a
    call ADC_measure
    bra $-2
    return
    
ADC_measure:
    bsf	  ADCON0,1,a ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, ADC_RH	; Result is complete - store 8 MSbits in ADC_RH
    movff ADRESL, ADC_RL	; store 2 LSbits in ADC_RL
    movff ADRESH, PORTD
    return
    

