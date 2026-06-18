set_motor_pwm macro ccp2, ccp3, ccp4, ccp5
    movlb 0xF
    movlw   ccp2    ; Duty Cycle for CCP2 (RB3) but3 off for setup
    movwf   CCPR2L,1 ;M1 F
    
    movlw   ccp3  ; Duty Cycle for CCP3 (RE0) but off for setup
    movwf   CCPR3L,1 ;M2 F
    
    movlw   ccp4  ; Duty Cycle for CCP1 (RD1) off for setup
    movwf   CCPR4L,1 ;M1 B
    
    movlw   ccp5  ; Duty Cycle for CCP5 (RE2) off for setup
    movwf   CCPR5L,1 ;M2 B
    movlb 0x0
endm

LLI_Entry:
    movff Sensor,prev_sensor
    btfsc Sensor,0
    bra right
    btfsc Sensor,4
    bra left
    btfsc Sensor,1
    bra slight_right
    btfsc Sensor,3
    bra slight_left
    btfsc Sensor,2
    bra straight
    bra search

right:		    
		    ;2_F ;1_F   ;2_B ;1_B
    set_motor_pwm   0x00,0xFA,  0x00,0x00
    return
    
slight_right:	  
		    ;1_F ;2_F   ;1_B ;2_B
    set_motor_pwm   0x00,0xAD,  0x00,0x00
    return
  
left:		    
    set_motor_pwm   0xFA,0x00,  0x00,0x00
    return
    
slight_left:	    
		    ;1_F ;2_F   ;1_B ;2_B
    set_motor_pwm   0xAD,0x00,  0x00,0x00
    return
   
straight:	    
		    ;1_F ;2_F   ;1_B ;2_B
    set_motor_pwm   0xBA,0xBA,  0x00,0x00
    return
 
stop:		    
		    ;2_F ;1_F   ;2_B ;1_B
    set_motor_pwm   0x00,0x00,  0x00,0x00
    return