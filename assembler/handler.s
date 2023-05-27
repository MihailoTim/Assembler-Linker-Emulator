symbol:
st %r1, [%r3]
st %r1, [%r4 + 0x05]
ld [%r5 + 0x03], %pc
beq %r1, %r2, symbol
.end