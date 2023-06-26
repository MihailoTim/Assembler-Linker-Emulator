.extern equ_data
.global A, data2
.equ A, equ_data + 8
data2:
.word equ_data, A
.end