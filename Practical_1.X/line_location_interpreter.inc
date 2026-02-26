
;try and start the logic at left sothat it runs sequancially as i intended it to
;-----------------------------------------------------------------------------
;Move left logic
left_logic:
    MOVF Sensor,w;Move Sensor value to wreg
    XORLW 0b10000;check if the wreg and the binary value match exactly
	BZ left;z will be 1 if they match and it will then branch HOPE p82
    MOVF Sensor,w
    XORLW 0b11000
	BZ left
    MOVF Sensor,w
    XORLW 0b11100
	BZ left
    MOVF Sensor,w
    XORLW 0b11110
	BZ left

;-----------------------------------------------------------------------------
;Move right logic
right_logic:
    MOVF Sensor,w
    XORLW 0b00001
	BZ right
    MOVF Sensor,w
    XORLW 0b00011
	BZ right
    MOVF Sensor,w
    XORLW 0b00111
	BZ right
    MOVF Sensor,w
    XORLW 0b01111
	BZ right
    
;-----------------------------------------------------------------------------
;Move slight left logic 
slight_left_logic:
    MOVF Sensor,w
    XORLW 0b01000
	BZ slight_left
    MOVF Sensor,w
    XORLW 0b01100
	BZ slight_left
    
;-----------------------------------------------------------------------------
;Move slight right logic
slight_right_logic:
    MOVF Sensor,w
    XORLW 0b00010
	BZ slight_right
    MOVF Sensor,w
    XORLW 0b00110
	BZ slight_right
    
;-----------------------------------------------------------------------------
;Move Straght logic 
straight_logic:
    MOVF Sensor,w
    XORLW 0b00100
	BZ straight
    MOVF Sensor,w
    XORLW 0b01110
	BZ straight
    
;-----------------------------------------------------------------------------
;Stop logic 
stop_logic:
    MOVF Sensor,w
    XORLW 0b11111
	BZ stop
    
;-----------------------------------------------------------------------------
;Move search logic
search_logic:;as the code tuns sequencially, this will always be the last option after non of the prev was chosen
    goto search
       
;-----------------------------------------------------------------------------
;restart main as no action was taken
    return

left: 
    MOVLW 0x01 ;turn on left led 0b00000001
    MOVWF LATA
    return
    
right:
    MOVLW 0x10;turn on right led 0b00010000
    MOVWF LATA
    return
  
slight_left:
    MOVLW 0x02;turn on sleft led 0b00000010
    MOVWF LATA
    return
    
slight_right:;turn on sright led 0b00001000
    MOVLW 0x08
    MOVWF LATA
    return
    
straight:
    MOVLW 0x04;turn on straight led 0b00000100
    MOVWF LATA
    return
 
stop:
    MOVLW 0x20;turn on stop led 0b00100000
    MOVWF LATA
    return
    
search:
    MOVLW 0x40;turn on search led 0b01000000
    MOVWF LATA
    return


