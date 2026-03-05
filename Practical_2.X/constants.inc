;==== Address Constants =====
tmp EQU   0x1
Delay1 EQU 0x2
Delay2 EQU 0x3
flash_counter EQU 0x4
Sensor equ 0x5
rcalib equ 0x6
rcolor equ 0x7
nav_col equ 0x8
r_col_det equ 0x9
r_RB4_do equ 0xB
 
s1r equ 0x21
s1g equ 0x22
s1b equ 0x23
 
s2r equ 0x24
s2g equ 0x25
s2b equ 0x26
 
s3r equ 0x27
s3g equ 0x28
s3b equ 0x29
 
s4r equ 0x2A
s4g equ 0x2B
s4b equ 0x2C
 
s5r equ 0x2D
s5g equ 0x2E
s5b equ 0x2F

W_R_Thres_min equ 0x30
W_R_Thres_max equ 0x31
W_G_Thres_min equ 0x32
W_G_Thres_max equ 0x33
W_B_Thres_min equ 0x34
W_B_Thres_max equ 0x35
 
R_R_Thres_min equ 0x36
R_R_Thres_max equ 0x38
R_G_Thres_min equ 0x39
R_G_Thres_max equ 0x3A
R_B_Thres_min equ 0x3B
R_B_Thres_max equ 0x3C
 
G_R_Thres_min equ 0x3D
G_R_Thres_max equ 0x3E
G_G_Thres_min equ 0x3F
G_G_Thres_max equ 0x40
G_B_Thres_min equ 0x41
G_B_Thres_max equ 0x42
 
B_R_Thres_min equ 0x43
B_R_Thres_max equ 0x44
B_G_Thres_min equ 0x45
B_G_Thres_max equ 0x46
B_B_Thres_min equ 0x47
B_B_Thres_max equ 0x48
 
K_R_Thres_min equ 0x49
K_R_Thres_max equ 0x4A
K_G_Thres_min equ 0x4B
K_G_Thres_max equ 0x4C
K_B_Thres_min equ 0x4D
K_B_Thres_max equ 0x4E
 
;===== Value Constants ===== 
RGB_R	EQU 0b00000001
RGB_G   EQU 0b00000010
RGB_B	EQU 0b00000100
RGB_W	EQU 0b00000111
H333ms EQU 0x60;0xD5
L333ms EQU 0xAA;0x55
ADCAQTH EQU 0xFD
ADCAQTL EQU 0xDD
Cycles_delay	EQU 1
	
;===== PORT Aliases =====
DUMP_REG EQU PORTD
RGB_REG EQU PORTC



