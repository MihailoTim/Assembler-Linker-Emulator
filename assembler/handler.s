.equ term_out, 0xFFFFFF00
.equ term_in, symbol1
.equ ascii_code, 84 # ascii(’T’)
.extern my_counter
.global symbol2
.section my_code_handler
.word 1,23,4,symbol3
st %r1, symbol
.end