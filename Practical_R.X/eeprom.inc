EEPROM_write:
    ;<editor-fold defaultstate="collapsed" desc="1. Generate start condition">
    CALL    I2C_START_CONDITION
    ;</editor-fold>

    ;<editor-fold defaultstate="collapsed" desc="2. Load & send the control byte/slave address (WRITE)">    
    MOVLW   WRITE_CONTROL
    MOVWF   i2c_byte
    CALL    I2C_WRITE
    
    ;--- Optional ACK check
    BTFSC   SSP1CON2,6       ; ACKSTAT = 1 means no ACK received
    GOTO    I2C_ERROR
    ;</editor-fold>

    ;<editor-fold defaultstate="collapsed" desc="3. Load & send the address">
    MOVF    eeprom_addr,W
    MOVWF   i2c_byte
    CALL    I2C_WRITE    

    ;--- Optional ACK check
    BTFSC   SSP1CON2,6
    GOTO    I2C_ERROR
    ;</editor-fold>

    ;<editor-fold defaultstate="collapsed" desc="4. Load & send the data">
    MOVF    i2c_char,W
    MOVWF   i2c_byte
    CALL    I2C_WRITE

    ;--- Optional ACK check
    BTFSC   SSP1CON2,6
    GOTO    I2C_ERROR
    
    CALL    I2C_STOP_CONDITION

    CALL    POLLING_WRITE_ACK
    
    return
;</editor-fold>
;Read
;<editor-fold defaultstate="collapsed" desc="Read from EEPROM">    


EEPROM_read:
    ;SET 4MHZ osccillator for I2C
    clrf i2c_len
    ;<editor-fold defaultstate="collapsed" desc="1. Generate start condition">
    CALL    I2C_START_CONDITION
    ;</editor-fold>

    ;<editor-fold defaultstate="collapsed" desc="2. Load & send the control byte/slave address: WRITE">
    MOVLW   WRITE_CONTROL
    MOVWF   i2c_byte
    
    CALL    I2C_WRITE

    BTFSC   SSP1CON2,6
    GOTO    I2C_ERROR
    ;</editor-fold>

    ;<editor-fold defaultstate="collapsed" desc="3. Load and send the EEPROM word address">
    MOVF    eeprom_addr,W
    MOVWF   i2c_byte
    CALL    I2C_WRITE

    BTFSC   SSP1CON2,6
    GOTO    I2C_ERROR
    ;</editor-fold>

    ;<editor-fold defaultstate="collapsed" desc="4. Restart to switch to receive mode">
    CALL    I2C_RESTART
    ;</editor-fold>

    ;<editor-fold defaultstate="collapsed" desc="5. Load and send the control byte/slave address: READ">
    MOVLW   READ_CONTROL
    MOVWF   i2c_byte
    CALL    I2C_WRITE

    BTFSC   SSP1CON2,6
    GOTO    I2C_ERROR
    ;</editor-fold>

Read_char:
    ;<editor-fold defaultstate="collapsed" desc="6. Read byte into POSTINC0">
    CALL   I2C_READ_BYTE
    ;</editor-fold>
    incf i2c_len
    movlw 0xF0
    cpfseq i2c_len
    bra $+4
    bra Last_Byte
    ;<editor-fold defaultstate="collapsed" desc="7. ACK all but last byte, NACK the last byte">
    MOVLW   0x0A
    CPFSEQ  tmp
    GOTO    More_Bytes

Last_Byte:
    CALL    I2C_SEND_NACK
    GOTO    Read_done

More_Bytes:
    CALL    I2C_SEND_ACK
    GOTO    Read_char
    ;</editor-fold>

Read_done:
    ;<editor-fold defaultstate="collapsed" desc="8. Stop">
    CALL    I2C_STOP_CONDITION
    ;Set osscillator back to 16 MHz
    ;</editor-fold>
    return
;</editor-fold>
;SUB routines
;<editor-fold defaultstate="collapsed" desc="SUB">
;<editor-fold defaultstate="collapsed" desc="1. Start Condidtion">
I2C_START_CONDITION:
    BCF     SSP1IF
    BSF     SSP1CON2,0      ; SEN = 1
wait_START:
    BTFSC   SSP1CON2,0
    BRA     wait_START
    RETURN
;</editor-fold>
;<editor-fold defaultstate="collapsed" desc="2. I2C Restart">
I2C_RESTART:
    BCF     SSP1IF
    BSF     SSP1CON2,1      ; RSEN = 1
wait_RESTART:
    BTFSC   SSP1CON2,1
    BRA     wait_RESTART
    RETURN
;</editor-fold>
;<editor-fold defaultstate="collapsed" desc="3. I2C Stop Concition">
I2C_STOP_CONDITION:
    BCF     SSP1IF
    BSF     SSP1CON2,2      ; PEN = 1
wait_STOP:
    BTFSC   SSP1CON2,2
    BRA     wait_STOP
    RETURN
;</editor-fold>
;<editor-fold defaultstate="collapsed" desc="4. Main I2C Write">
I2C_WRITE:
    BTFSC   SSP1STAT,0      ; BF = 1 means buffer full
    GOTO    I2C_WRITE
    BCF     SSP1IF
    MOVF    i2c_byte,0,0
    MOVWF   SSP1BUF
wait_WRITE:
    BTFSS   SSP1IF
    BRA     wait_WRITE
    RETURN
;</editor-fold>
;<editor-fold defaultstate="collapsed" desc="5. Read Byte">
I2C_READ_BYTE:
    BCF     SSP1IF
    BSF     SSP1CON2,3      ; RCEN = 1, enable receive mode
WAIT1_READ:
    BTFSS   SSP1IF
    BRA     WAIT1_READ
    BTFSS   SSP1STAT,0      ; BF must be set when byte is received
    BRA     WAIT1_READ
    MOVF    SSP1BUF,W
  
    MOVWF   POSTINC0
    movwf   tmp
    
    RETURN
    
;</editor-fold>
;<editor-fold defaultstate="collapsed" desc="6. Send ACK">
I2C_SEND_ACK:
    BCF     SSP1CON2,5      ; ACKDT = 0 -> ACK
    BCF     SSP1IF
    BSF     SSP1CON2,4      ; ACKEN = 1
WAIT_ACK:
    BTFSS   SSP1IF
    BRA     WAIT_ACK
    RETURN
;</editor-fold>
;<editor-fold defaultstate="collapsed" desc="8. Send NACK">
I2C_SEND_NACK:
    BSF     SSP1CON2,5      ; ACKDT = 1 -> NACK
    BCF     SSP1IF
    BSF     SSP1CON2,4      ; ACKEN = 1
    ;movlw 255
    ;movwf tmp
WAIT_NACK:
    ;decfsz tmp
    ;bra $+4
    ;return
    
    BTFSS   SSP1IF
    BRA     WAIT_NACK
    RETURN
;</editor-fold>
;<editor-fold defaultstate="collapsed" desc="8. Poll write ACK">
POLLING_WRITE_ACK:
Poll_Loop:
    CALL    I2C_START_CONDITION

    MOVLW   WRITE_CONTROL
    MOVWF   i2c_byte
    CALL    I2C_WRITE

    ; ACKSTAT = 1 => EEPROM still busy
    BTFSC   SSP1CON2,6
    GOTO    Poll_NotReady

Poll_Ready:
    CALL    I2C_STOP_CONDITION
    RETURN

Poll_NotReady:
    CALL    I2C_STOP_CONDITION
    GOTO    Poll_Loop
;</editor-fold>
;<editor-fold defaultstate="collapsed" desc="9. I2C Error">
I2C_ERROR:
    return
;</editor-fold>
;<editor-fold defaultstate="collapsed" desc="10. Delay">
DELAY:
    MOVLW   0xFF
    MOVWF   Delay2
LOOP1:
    MOVLW   0xFF
    MOVWF   Delay1
LOOP2:
    DECFSZ  Delay1
    GOTO    LOOP2
    DECFSZ  Delay2
    GOTO    LOOP1
    RETURN
;</editor-fold>
;</editor-fold>  
    
EEPROM_startup_message:
    lfsr 0,i2c_sto_addr
    movlw eeprom_startmsg_addr
    movwf eeprom_addr
    call EEPROM_read
    return
    
EEPROM_menu_message:
    lfsr 0,i2c_sto_addr
    movlw eeprom_menu_addr
    movwf eeprom_addr
    call EEPROM_read
    return
    
EEPROM_slogan_message:
    lfsr 0,i2c_sto_addr
    movlw eeprom_slogan_addr
    movwf eeprom_addr
    call EEPROM_read
    return

    
write_table_to_eeprom:
    TBLRD*+
    movff TABLAT,i2c_char
    call EEPROM_write
    incf eeprom_addr
    movlw 0xA
    cpfseq i2c_char
    bra write_table_to_eeprom
    return
    
write_lfsr_to_eeprom:
    movff POSTINC0,i2c_char
    call EEPROM_write
    incf eeprom_addr
    movlw 0xA
    cpfseq i2c_char
    bra write_lfsr_to_eeprom
    return
    
    
    
EEPROM_default:
    movlw highword(startup_message)
    movwf TBLPTRU
    movlw high(startup_message)
    movwf TBLPTRH
    movlw low(startup_message)
    movwf TBLPTRL
    movlw eeprom_startmsg_addr
    movwf eeprom_addr
    call write_table_to_eeprom

	
    movlw highword(menu_message)
    movwf TBLPTRU
    movlw high(menu_message)
    movwf TBLPTRH
    movlw low(menu_message)
    movwf TBLPTRL
    movlw eeprom_menu_addr
    movwf eeprom_addr
    call write_table_to_eeprom
    
    movlw highword(default_slogan_message)
    movwf TBLPTRU
    movlw high(default_slogan_message)
    movwf TBLPTRH
    movlw low(default_slogan_message)
    movwf TBLPTRL
    movlw eeprom_slogan_addr
    movwf eeprom_addr
    call write_table_to_eeprom
    return
    
menu_message:
    DB  "Choose your MARV mode...", 0x0D
    DB  "(C)olour", 0x0D
    DB  "(R)eference", 0x0D
    DB  "(A)ttack", 0x0D
    DB  "(S)imulate race", 0x0D
    DB  "(H)otload EEPROM", 0x0D, 0x0A, 0x0
    
startup_message:
    db "Jy kan maar glo Jessie baby daar's geen ander een",0xD,0xA,0x0  
    
default_slogan_message:
    db "JESSICA !!",0xD,0xA