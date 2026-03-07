ISRL:
    nop
    nop
    nop
    retfie
    
ISRH:
    btfss INTCON,0,a ;check if RBIF is 1 else retfie
    goto ISRH_done
   
    btfss PORTB,4,a ;check if RB4 is set
    bra Reset_RB4
    tstfsz rcalib,a
    bra inter_calib
    bra RB4_do
    inter_calib:
	bsf rcalib,1,a
	bra Reset_RB4
    RB4_do:
	tstfsz r_RB4_do,a
	bra RB4_dump_value
	bra RB4_dump_value
	RB4_dump_color:
	    movff r_col_det,DUMP_REG
	    clrf r_RB4_do,a
	    bra Reset_RB4
	RB4_dump_value:
	    ;NOTHING DUMPING !
	    wait_timer H333ms,L333ms
	    wait_timer H333ms,L333ms
	    wait_timer H333ms,L333ms
	    wait_timer H333ms,L333ms
	    wait_timer H333ms,L333ms
	    wait_timer H333ms,L333ms
	    incf r_RB4_do,a
	    
	    bra Reset_RB4
    Reset_RB4:
	movf  PORTB,w,a
	bcf   INTCON,0,a
	    
    ISRH_done:
	retfie


