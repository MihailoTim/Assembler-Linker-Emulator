.extern symbol2
symbol:
ld 0xFF, %r4
ld symbol, %r1
ld $symbol, %r2
ld $0xEA, %sp
ld %pc, %sp
ld [%r5], %r0
ld [%r1 + 0x05], %r1
ld [%sp + symbol2], %pc

.end