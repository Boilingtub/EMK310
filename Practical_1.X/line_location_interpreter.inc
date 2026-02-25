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
    MOVFF Sensor,LATD
;-----------------------------------------------------------------------------
;Move left logic
    MOVF Sensor,w
    XORLW 0b10000
	BZ left
    MOVF Sensor,w
    XORLW 0b11000
	BZ left
    MOVF Sensor,w
    XORLW 0b11100
	BZ left
    MOVF Sensor,w
    XORLW 0b11110
	BZ left

    
    ;MOVF Sensor,w
    ;ANDLW 0b00010000
    ;BTFSS STATUS,2;test if the left most bit is 0
	;GOTO test_right

   ; MOVF Sensor,w
    ;ANDLW 0b00001111
    ;BZ left;tets if z=0,meaning only left sensor is on
    
    
    ;MOVWF temp;
    ;;ANDLW 0b00000111
;     BZ left;test if z=0,meaning left 2 sensor on
;     
;     
;     ANDLW 0b00001111;value used to check if only left sensor is trigered
;     ;and sensor and w and the result is stored in w
;     BZ left
;     
;     MOVWF temp
;     
;     BZ left;z is 1 if w = 0's and z is 0 when w has one 1, if 1 goto left
    
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
    
    ;MOVF Sensor,w
    ;ANDLW 0b00011110;value used to check if only right sensor is trigered
    ;BTFSC STATUS,2;checks the value of z, if 0 skip next line
    ;GOTO right
    
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
    MOVLW 0x01 ;turn on D5 led 0b00000001
    MOVWF LATA
    GOTO main
    
right:
    MOVLW 0x10;turn on D2 led 0b00010000
    MOVWF LATA
    GOTO main
  
slight_left:
    MOVLW 0x02
    MOVWF LATA
    GOTO main
    
slight_right:
    MOVLW 0x08
    MOVWF LATA
    GOTO main
    
straight:
    MOVLW 0x04;turn on D2 led 0b00000100
    MOVWF LATA
    GOTO main
 
stop:
    MOVLW 0x20
    MOVWF LATA
    GOTO main
    
search:
    MOVLW 0x40
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


