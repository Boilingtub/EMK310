toggle_RGB macro out_port, addr_port, rgb_val
    movff   addr_port,  WREG ;Move Port B into Working register 
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
    bsf	  ADCON0,1,a ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, ADC_RH 	; Result is complete - store 2 MSbits in
    movff ADRESL, ADC_RL	; RESULTHI and 8 LSbits in RESULTLO
    movff ADRESL, reg_col
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
    calibrate_RGB rRGB1, flash_counter, 6, PORTB, PORTB_val, RGB_R
    calibrate_RGB rRGB2, flash_counter, 6, PORTB, PORTB_val, RGB_G
    calibrate_RGB rRGB3, flash_counter, 6, PORTB, PORTB_val, RGB_B
    calibrate_RGB rRGB4, flash_counter, 6, PORTB, PORTB_val, RGB_W
    return
        
RGB_calibrate_test:
    movlw   0b00000011
    movwf   PORTC,a
    bra $-2
    return
    
ADC_measure:
    bsf	  ADCON0,1,a ;Start conversion
    btfsc ADCON0,1,a ;Is conversion done?, NO, test again
    bra   $-2
    movff ADRESH, ADC_RH	; Result is complete - store 8 MSbits in ADC_RH
    movff ADRESL, ADC_RL	; store 2 LSbits in ADC_RL
    return
    

