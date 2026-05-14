    title	"Our second assembler program"
    PROCESSOR	18F45K22
    
    ; CONFIG1H
    CONFIG  FOSC = INTIO67        ; Oscillator Selection bits (Internal oscillator block)
				  ; There is a how-to tutorial on the configuration bits
  
    
    #include    <xc.inc>
    #include    "pic18f45k22.inc"
;
; -------------	
; PROGRAM START	
; -------------
;
    PSECT code,abs //Start of main code.
    org 		0h 			; startup address = 0000h

    ; Initialize Port A (check datasheet for example)
    MOVLB		0xF		; Why would one want to manipulate the BSR?
    CLRF 		PORTA,a 	; Initialize PORTA by clearing output data latches
    CLRF 		LATA,a 		; Alternate method to clear output data latches
    CLRF		ANSELA,b 	; Configure I/O (Check address of ANSELA)
    movlw		0b00001111
    movwf 		TRISA,a		; All digital outputs
    MOVLB		0x00		
		
		
start: 	
    btfss   PORTA,0,a
    bra $+4
    bra $+2
    goto 		start 		; do this loop forever
	
    ; Read chapter on I/O ports in the datasheet 
    end			
