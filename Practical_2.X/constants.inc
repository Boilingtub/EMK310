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

;===== Sensor 1 Values =====
S1_W_R_Thres_min equ 0x30
S1_W_R_Thres_max equ 0x31
S1_W_G_Thres_min equ 0x32
S1_W_G_Thres_max equ 0x33
S1_W_B_Thres_min equ 0x34
S1_W_B_Thres_max equ 0x35
 
S1_R_R_Thres_min equ 0x36
S1_R_R_Thres_max equ 0x37
S1_R_G_Thres_min equ 0x38
S1_R_G_Thres_max equ 0x39
S1_R_B_Thres_min equ 0x3A
S1_R_B_Thres_max equ 0x3B
 
S1_G_R_Thres_min equ 0x3C
S1_G_R_Thres_max equ 0x3D
S1_G_G_Thres_min equ 0x3E
S1_G_G_Thres_max equ 0x3F
S1_G_B_Thres_min equ 0x40
S1_G_B_Thres_max equ 0x41
 
S1_B_R_Thres_min equ 0x42
S1_B_R_Thres_max equ 0x43
S1_B_G_Thres_min equ 0x44
S1_B_G_Thres_max equ 0x45
S1_B_B_Thres_min equ 0x46
S1_B_B_Thres_max equ 0x47
 
S1_K_R_Thres_min equ 0x48
S1_K_R_Thres_max equ 0x49
S1_K_G_Thres_min equ 0x4A
S1_K_G_Thres_max equ 0x4B
S1_K_B_Thres_min equ 0x4C
S1_K_B_Thres_max equ 0x4D
 
;===== Sensor 2 values ===== 
S2_W_R_Thres_min equ 0x4E
S2_W_R_Thres_max equ 0x4F
S2_W_G_Thres_min equ 0x50
S2_W_G_Thres_max equ 0x51
S2_W_B_Thres_min equ 0x52
S2_W_B_Thres_max equ 0x53
 
S2_R_R_Thres_min equ 0x54
S2_R_R_Thres_max equ 0x55
S2_R_G_Thres_min equ 0x56
S2_R_G_Thres_max equ 0x57
S2_R_B_Thres_min equ 0x58
S2_R_B_Thres_max equ 0x59
 
S2_G_R_Thres_min equ 0x5A
S2_G_R_Thres_max equ 0x5B
S2_G_G_Thres_min equ 0x5C
S2_G_G_Thres_max equ 0x5D
S2_G_B_Thres_min equ 0x5E
S2_G_B_Thres_max equ 0x5F
 
S2_B_R_Thres_min equ 0x60
S2_B_R_Thres_max equ 0x61
S2_B_G_Thres_min equ 0x62
S2_B_G_Thres_max equ 0x63
S2_B_B_Thres_min equ 0x64
S2_B_B_Thres_max equ 0x65
 
S2_K_R_Thres_min equ 0x66
S2_K_R_Thres_max equ 0x67
S2_K_G_Thres_min equ 0x68
S2_K_G_Thres_max equ 0x69
S2_K_B_Thres_min equ 0x6A
S2_K_B_Thres_max equ 0x6B
 
;===== Sensor 3 values ===== 
S3_W_R_Thres_min equ 0x6C
S3_W_R_Thres_max equ 0x6D
S3_W_G_Thres_min equ 0x6E
S3_W_G_Thres_max equ 0x6F
S3_W_B_Thres_min equ 0x70
S3_W_B_Thres_max equ 0x71
 
S3_R_R_Thres_min equ 0x72
S3_R_R_Thres_max equ 0x73
S3_R_G_Thres_min equ 0x74
S3_R_G_Thres_max equ 0x75
S3_R_B_Thres_min equ 0x76
S3_R_B_Thres_max equ 0x77
 
S3_G_R_Thres_min equ 0x78
S3_G_R_Thres_max equ 0x79
S3_G_G_Thres_min equ 0x7A
S3_G_G_Thres_max equ 0x7B
S3_G_B_Thres_min equ 0x7C
S3_G_B_Thres_max equ 0x7D
 
S3_B_R_Thres_min equ 0x7E
S3_B_R_Thres_max equ 0x7F
S3_B_G_Thres_min equ 0x80
S3_B_G_Thres_max equ 0x81
S3_B_B_Thres_min equ 0x82
S3_B_B_Thres_max equ 0x83
 
S3_K_R_Thres_min equ 0x84
S3_K_R_Thres_max equ 0x85
S3_K_G_Thres_min equ 0x86
S3_K_G_Thres_max equ 0x87
S3_K_B_Thres_min equ 0x88
S3_K_B_Thres_max equ 0x89
 
;===== Sensor 4 values ===== 
S4_W_R_Thres_min equ 0x8A
S4_W_R_Thres_max equ 0x8B
S4_W_G_Thres_min equ 0x8C
S4_W_G_Thres_max equ 0x8D
S4_W_B_Thres_min equ 0x8E
S4_W_B_Thres_max equ 0x8F
 
S4_R_R_Thres_min equ 0x90
S4_R_R_Thres_max equ 0x91
S4_R_G_Thres_min equ 0x92
S4_R_G_Thres_max equ 0x93
S4_R_B_Thres_min equ 0x94
S4_R_B_Thres_max equ 0x95
 
S4_G_R_Thres_min equ 0x96
S4_G_R_Thres_max equ 0x97
S4_G_G_Thres_min equ 0x98
S4_G_G_Thres_max equ 0x99
S4_G_B_Thres_min equ 0x9A
S4_G_B_Thres_max equ 0x9B

S4_B_R_Thres_min equ 0x9C
S4_B_R_Thres_max equ 0x9D
S4_B_G_Thres_min equ 0x9E
S4_B_G_Thres_max equ 0x9F
S4_B_B_Thres_min equ 0xA0
S4_B_B_Thres_max equ 0xA1
 
S4_K_R_Thres_min equ 0xA2
S4_K_R_Thres_max equ 0xA3
S4_K_G_Thres_min equ 0xA4
S4_K_G_Thres_max equ 0xA5
S4_K_B_Thres_min equ 0xA6
S4_K_B_Thres_max equ 0xA7
 
;===== Sensor 5 values ===== 
S5_W_R_Thres_min equ 0xA8
S5_W_R_Thres_max equ 0xA9
S5_W_G_Thres_min equ 0xAA
S5_W_G_Thres_max equ 0xAB
S5_W_B_Thres_min equ 0xAC
S5_W_B_Thres_max equ 0xAD

S5_R_R_Thres_min equ 0xAE
S5_R_R_Thres_max equ 0xAF
S5_R_G_Thres_min equ 0xB0
S5_R_G_Thres_max equ 0xB1
S5_R_B_Thres_min equ 0xB2
S5_R_B_Thres_max equ 0xB3

S5_G_R_Thres_min equ 0xB4
S5_G_R_Thres_max equ 0xB5
S5_G_G_Thres_min equ 0xB6
S5_G_G_Thres_max equ 0xB7
S5_G_B_Thres_min equ 0xB8
S5_G_B_Thres_max equ 0xB9

S5_B_R_Thres_min equ 0xBA
S5_B_R_Thres_max equ 0xBB
S5_B_G_Thres_min equ 0xBC
S5_B_G_Thres_max equ 0xBD
S5_B_B_Thres_min equ 0xBE
S5_B_B_Thres_max equ 0xBF

S5_K_R_Thres_min equ 0xC0
S5_K_R_Thres_max equ 0xC1
S5_K_G_Thres_min equ 0xC2
S5_K_G_Thres_max equ 0xC3
S5_K_B_Thres_min equ 0xC4
S5_K_B_Thres_max equ 0xC5

;===== Value Constants ===== 
Thres_var equ 16
H333ms EQU 0x60;0xD5
L333ms EQU 0xAA;0x55
ADCAQTH EQU 0xFD
ADCAQTL EQU 0xDD
	
;===== PORT Aliases =====
DUMP_REG EQU PORTD
RGB_REG EQU PORTC



