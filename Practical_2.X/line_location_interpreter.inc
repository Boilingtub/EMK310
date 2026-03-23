;try and start the logic at left sothat it runs sequancially as i intended it to
;-----------------------------------------------------------------------------
;Move left logic
LLI_Entry:
left_logic:
    movf Sensor,0;Move Sensor value to wreg
    XORLW 0b10000000;check if the wreg and the binary value match exactly
	BZ left;z will be 1 if they match and it will then branch HOPE p82
    movf Sensor,0
    XORLW 0b11000000
	BZ left
    movf Sensor,0
    XORLW 0b11100000
	BZ left
    movf Sensor,0
    XORLW 0b11110000
	BZ left

;-----------------------------------------------------------------------------
;Move right logic
right_logic:
    MOVF Sensor,0
    XORLW 0b00001000
	BZ right
    MOVF Sensor,0
    XORLW 0b00011000
	BZ right
    MOVF Sensor,0
    XORLW 0b00111000
	BZ right
    MOVF Sensor,0
    XORLW 0b01111000
	BZ right
    
;-----------------------------------------------------------------------------
;Move slight left logic 
slight_left_logic:
    MOVF Sensor,0
    XORLW 0b01000000
	BZ slight_left
    MOVF Sensor,0
    XORLW 0b01100000
	BZ slight_left
   
;-----------------------------------------------------------------------------
;Move slight right logic
slight_right_logic:
    MOVF Sensor,0
    XORLW 0b00010000
	BZ slight_right
    MOVF Sensor,0
    XORLW 0b00110000
	BZ slight_right
    
;-----------------------------------------------------------------------------
;Move Straght logic 
straight_logic:
    MOVF Sensor,0
    XORLW 0b00100000
	BZ straight
    MOVF Sensor,0
    XORLW 0b01110000
	BZ straight
    
;-----------------------------------------------------------------------------
;Stop logic 
;stop_logic:
;    MOVF Sensor,w
;    XORLW 0b11111000
;	BZ stop
    
;-----------------------------------------------------------------------------
;Move search logic 
search_logic:;as the code tuns sequencially, this will always be the last option after non of the prev was chosen
    goto search
       
;-----------------------------------------------------------------------------
;restart main as no action was taken
    return

left: 
    MOVLW 0x01 ;turn on left led 0b00000001
    MOVWF LATD,a
    return
    
right:
    MOVLW 0x10;turn on right led 0b00010000
    MOVWF LATD,a
    return
  
slight_left:
    MOVLW 0x02;turn on sleft led 0b00000010
    MOVWF LATD,a
    return
    
slight_right:;turn on sright led 0b00001000
    MOVLW 0x08
    MOVWF LATD,a
    return
    
straight:
    MOVLW 0x04;turn on straight led 0b00000100
    MOVWF LATD,a
    return
 
stop:
    MOVLW 0x20;turn on stop led 0b00100000
    MOVWF LATD,a
    return
    
search:
    MOVLW 0x40;turn on search led 0b01000000
    MOVWF LATD,a
    return    