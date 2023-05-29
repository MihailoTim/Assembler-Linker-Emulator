symbol1:
.skip 20
.section data
.skip 10
symbol2:
jmp symbol1
beq %r1, %r11, symbol1
bne %r1, %r12, symbol2
bgt %r1, %r12, symbol3
beq %r10, %r10, 0xF000
.skip 20
.section text
.skip 12
symbol4:
jmp symbol1
beq %r1, %r11, symbol1
bne %r1, %r12, symbol2
bgt %r1, %r12, symbol3
beq %r10, %r10, 0xF000
.skip 20
symbol3:
.end
