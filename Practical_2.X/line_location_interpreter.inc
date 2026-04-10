set_motor_pwm macro ccp2, ccp3, ccp4, ccp5
    movlw   ccp2    ; Duty Cycle for CCP2 (RB3) but3 off for setup
    banksel CCPR2L
    movwf   CCPR2L,1 ;M1 F
    
    movlw   ccp3  ; Duty Cycle for CCP3 (RE0) but off for setup
    banksel CCPR3L
    movwf   CCPR3L,1 ;M2 F
    
    movlw   ccp4  ; Duty Cycle for CCP4 (RD1) off for setup
    banksel CCPR4L
    movwf   CCPR4L,1 ;M1 B
    
    movlw   ccp5  ; Duty Cycle for CCP5 (RE2) off for setup
    banksel CCPR5L
    movwf   CCPR5L,1 ;M2 B
    movlb 0
endm
;try and start the logic at left sothat it runs sequancially as i intended it to
;-----------------------------------------------------------------------------
;Move left logic
LLI_Entry:
left_logic:
    movf Sensor,0,0;Move Sensor value to wreg
    XORLW 0b00000001;check if the wreg and the binary value match exactly
	BZ left;z will be 1 if they match and it will then branch HOPE p82
    movf Sensor,0,0
    XORLW 0b00000011
	BZ left
    movf Sensor,0,0
    XORLW 0b00000111
	BZ left
    movf Sensor,0,0
    XORLW 0b00001111
	BZ left

;-----------------------------------------------------------------------------
;Move right logic
right_logic:
    MOVF Sensor,0,0
    XORLW 0b00010000
	BZ right
    MOVF Sensor,0,0
    XORLW 0b00011000
	BZ right
    MOVF Sensor,0,0
    XORLW 0b00011100
	BZ right
    MOVF Sensor,0,0
    XORLW 0b00011110
	BZ right
    
;-----------------------------------------------------------------------------
;Move slight left logic 
slight_left_logic:
    MOVF Sensor,0,0
    XORLW 0b00000010
	BZ slight_left
    MOVF Sensor,0,0
    XORLW 0b00000110
	BZ slight_left
   
;-----------------------------------------------------------------------------
;Move slight right logic
slight_right_logic:
    MOVF Sensor,0,0
    XORLW 0b00001000
	BZ slight_right
    MOVF Sensor,0,0
    XORLW 0b00011000
	BZ slight_right
    
;-----------------------------------------------------------------------------
;Move Straght logic 
straight_logic:
    MOVF Sensor,0,0
    XORLW 0b00000100
	BZ straight
    MOVF Sensor,0,0
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

right:		    ;2_F ;1_F   ;2_B ;1_B
    set_motor_pwm   0x00,0xFA,  0x00,0x00
    return
    
slight_right:	    ;2_F ;1_F   ;2_B ;1_B
    set_motor_pwm   0x00,0xAD,  0x00,0x00
    return
  
left:		    ;1_F ;2_F   ;1_B ;2_B
    set_motor_pwm   0xFA,0x00,  0x00,0x00
    return
    
slight_left:	    ;2_F ;1_F   ;2_B ;1_B
    set_motor_pwm   0xAA,0x00,  0x00,0x00
    return
   
straight:	    ;2_F ;1_F   ;2_B ;1_B
    set_motor_pwm   0xFA,0xFA,  0x00,0x00
    return
 
stop:		    ;2_F ;1_F   ;2_B ;1_B
    set_motor_pwm   0x00,0x00,  0x00,0x00
    return
    
search:		    ;2_F ;1_F   ;2_B ;1_B
    set_motor_pwm   0xAF,0x00,  0x00,0x00
    return    