ISRL:
    nop
    nop
    nop
    retfie
    
ISRH:
    btfss INTCON,0,a ;check if RBIF is 1 else retfie
    goto ISRH_done
   
    btfsc PORTB,4,a ;check if RB4 is set
    bra Reset_RB4
    tstfsz rcalib,a
    rlncf rcalib,a
    Reset_RB4:
	movf  PORTB,w,a
	bcf   INTCON,0,a
	    
    ISRH_done:
	retfie