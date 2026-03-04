wait_n_cycles macro num, l_addr1, l_addr2
    movlw   num
    movwf   l_addr1, a
    movwf   l_addr2, a
    decfsz  l_addr1, a 
    bra     $-2
    movwf   l_addr1,a 
    decfsz  l_addr2, a
    bra     $-8
endm

wait_timer macro th,tl ;Wait_time = 0xFFFF - (OxFFFF/(Hz*2))
    ;YYXX = 65535 - (time required)/((4/fosc)*prescaler)
    ;YYXX = 44723 -> 0XAEB3 in hex	
    movlw 0x83
    movwf T0CON, c ;c specifies that it is the access bank
    ;preloading the timer
    movlw th	;Timer High bits
    movwf TMR0H, c
    movlw tl	;Timer Low Bits
    movwf TMR0L, c	
    ;clearing the timer overflow flag and turning on the timer
    bcf INTCON, 2, c
    bsf T0CON, 7, c
    
    btfss INTCON, 2, c
    bra $-4		
    bcf T0CON, 7, c ; timer is turned off
endm


