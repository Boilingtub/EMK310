lli_init:
    MOVLW 0xFF
    MOVWF Sensor
    
press:
    BTFSC PORTB,4
    bra press
 
release:
    BTFSS PORTB,4
    bra release
    
    call input_stepper;function to run through LUT
    MOVFF Sensor,LATD;outputs the current stage of lut to led's
;-----------------------------------------------------------------------------
;Move left logic
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
    MOVF Sensor,w
    XORLW 0b01000
	BZ slight_left
    MOVF Sensor,w
    XORLW 0b01100
	BZ slight_left
    
;-----------------------------------------------------------------------------
;Move slight right logic 
    MOVF Sensor,w
    XORLW 0b00010
	BZ slight_right
    MOVF Sensor,w
    XORLW 0b00110
	BZ slight_right
    
;-----------------------------------------------------------------------------
;Move Straght logic 
    MOVF Sensor,w
    XORLW 0b00100
	BZ straight
    MOVF Sensor,w
    XORLW 0b01110
	BZ straight
    
;-----------------------------------------------------------------------------
;Stop logic 
    MOVF Sensor,w
    XORLW 0b11111
	BZ stop
    
;-----------------------------------------------------------------------------
;Move search logic 
    goto search
       
;-----------------------------------------------------------------------------
;restart main as no action was taken
    BRA main

left: 
    MOVLW 0x01 ;turn on left led 0b00000001
    MOVWF LATA
    GOTO main
    
right:
    MOVLW 0x10;turn on right led 0b00010000
    MOVWF LATA
    GOTO main
  
slight_left:
    MOVLW 0x02;turn on sleft led 0b00000010
    MOVWF LATA
    GOTO main
    
slight_right:;turn on sright led 0b00001000
    MOVLW 0x08
    MOVWF LATA
    GOTO main
    
straight:
    MOVLW 0x04;turn on straight led 0b00000100
    MOVWF LATA
    GOTO main
 
stop:
    MOVLW 0x20;turn on stop led 0b00100000
    MOVWF LATA
    GOTO main
    
search:
    MOVLW 0x40;turn on search led 0b01000000
    MOVWF LATA
    GOTO main
    
input_stepper:;to run trough lut table but will have to reset regester here
    INCF Sensor, f
    ;BCf LATA,4;clears the led output, but only for signle led
    MOVF Sensor,w
    XORLW 0x20;to test when the sensor hits 0b00100000 reset to 0
	BZ reset_var
    CLRF LATA;clear the latch to make led's go off 
   return
   
reset_var:
    MOVLW 0x00
    MOVWF Sensor
    return


