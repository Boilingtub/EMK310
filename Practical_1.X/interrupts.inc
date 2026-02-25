ISRH:
    retfie
ISRL:
    btfss INTCON,0,a ;check if RBIF is 1 else retfie
    goto ISRL_done
   
    btfss PORTB,4,a ;check if RB4 is set
    bra Reset_RB4
    movff ADRESH,DUMP_REG
    Reset_RB4:
	movf  PORTB,w,a
	bcf   INTCON,0,a
	
    btfss PORTB,5,a ;check if RB5 is not set
	bra Reset_RB5
	movff ADRESH,DUMP_REG
	Reset_RB5:
	    movf  PORTB,w,a
	    bcf   INTCON,0,a
	    bra ISRL_done
	    
    ISRL_done:
	retfie