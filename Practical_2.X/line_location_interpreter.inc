set_motor_pwm macro ccp2, ccp3, ccp4, ccp5
    movlw   ccp2    ; Duty Cycle for CCP1 (RB3) but3 off for setup
    banksel CCPR2L
    movwf   CCPR2L,b
    
    movlw   ccp3  ; Duty Cycle for CCP2 (RE0) but off for setup
    banksel CCPR3L
    movwf   CCPR3L,b
    
    movlw   ccp4  ; Duty Cycle for CCP3 (RB0) off for setup
    banksel CCPR4L
    movwf   CCPR4L,b
    
    movlw   ccp5  ; Duty Cycle for CCP4 (RE2) off for setup
    banksel CCPR5L
    movwf   CCPR5L,b
endm
;try and start the logic at left sothat it runs sequancially as i intended it to
;-----------------------------------------------------------------------------
;Move left logic
LLI_Entry:
left_logic:
    movf Sensor,0;Move Sensor value to wreg
    XORLW 0b00010000;check if the wreg and the binary value match exactly
	BZ left;z will be 1 if they match and it will then branch HOPE p82
    movf Sensor,0
    XORLW 0b00011000
	BZ left
    movf Sensor,0
    XORLW 0b00011100
	BZ left
    movf Sensor,0
    XORLW 0b00011110
	BZ left

;-----------------------------------------------------------------------------
;Move right logic
right_logic:
    MOVF Sensor,0
    XORLW 0b00000001
	BZ right
    MOVF Sensor,0
    XORLW 0b00000011
	BZ right
    MOVF Sensor,0
    XORLW 0b00000111
	BZ right
    MOVF Sensor,0
    XORLW 0b00001111
	BZ right
    
;-----------------------------------------------------------------------------
;Move slight left logic 
slight_left_logic:
    MOVF Sensor,0
    XORLW 0b00001000
	BZ slight_left
    MOVF Sensor,0
    XORLW 0b00001100
	BZ slight_left
   
;-----------------------------------------------------------------------------
;Move slight right logic
slight_right_logic:
    MOVF Sensor,0
    XORLW 0b00000010
	BZ slight_right
    MOVF Sensor,0
    XORLW 0b00000110
	BZ slight_right
    
;-----------------------------------------------------------------------------
;Move Straght logic 
straight_logic:
    MOVF Sensor,0
    XORLW 0b00000100
	BZ straight
    MOVF Sensor,0
    XORLW 0b00001110
	BZ straight
    
;-----------------------------------------------------------------------------
;Stop logic 
;stop_logic:
;    MOVF Sensor,w
;    XORLW 0b00011111
;	BZ stop
    
;-----------------------------------------------------------------------------
;Move search logic 
search_logic:;as the code tuns sequencially, this will always be the last option after non of the prev was chosen
    goto search
       
;-----------------------------------------------------------------------------
;restart main as no action was taken
    return

left: 
    set_motor_pwm 0x00,0xAF,0xFA,0x00
    return
    
right:
    set_motor_pwm 0xFA,0x00,0x00,0xAF
    return
  
slight_left:
    set_motor_pwm 0x7D,0x00,0xFA,0x00
    return
    
slight_right:
    set_motor_pwm 0xFA,0x00,0x7D,0x00
    return
    
straight:
    set_motor_pwm 0xFA,0x00,0xFA,0x00
    return
 
stop:
    set_motor_pwm 0x00,0x00,0x00,0x00
    return
    
search:
    set_motor_pwm 0x00,0x7D,0x00,0x7D
    return    