.global equ_data, literal
.equ equ_data, data + 4
.equ literal, 0x3
.section data
.word 1,2,3,4, equ_data
.end