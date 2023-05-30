symbol1:
.skip 20
.section data
.skip 10
symbol2:
jmp symbol1
beq %r1, %r11, 0xFFFF
bne %r1, %r12, 0x1234
bgt %r1, %r12, symbol3
.skip 50
.word 0xFFFFFFFF
beq %r10, %r10, 0xF000
call symbol2
bgt %r1, %r12, symbol2
.skip 20
symbol3:
.end
