symbol:
jmp symbol
beq %r1, %r2, symbol
bne %r5, %r10, symbol
bgt %sp, %pc, symbol
xchg %r1, %r2
add %r0, %r7
sub %sp, %r7
add %r0, %pc
not %pc
and %r0, %r5
or %r14, %sp
xor %r1, %r1
shl %r8, %r7
shr %r10, %r11
.end