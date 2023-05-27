.global symbol1
symbol1:
.section text
symbol2:
jmp symbol3
beq %r1, %r2,  symbol2
bne %r0, %pc, symbol3
bgt %r3, %r7,  symbol4
jmp symbol3
symbol3:
.section data
.skip 20
symbol4:
.end