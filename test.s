start:
not %r1 # r1 = -1
sub %r1, %r2 # r2 = 1
add %r2, %r3 # r3 = 1
or %r1, %r4 # r4 = -1
or %r2, %r5 # r5 = 1
xor %r1, %r5 # r5 = -2
mul %r5, %r4 # r4 = 2
add %r4, %r6
add %r6, %r6
div %r5, %r6 # r6 = -2
and %r6, %r1 # r1 = -2
xchg %r1, %r3 # r1 = 1, r3 = -2
shl %r1, %r3 # r3 = -4
shr %r1, %r5 # r5 = -1
ld $11, %r7
st %r7, 0x1000000
ld 0x10000000, %r8
csrwr %r7, %status
csrrd %status, %r11
csrwr %r11, %cause
push %r11
pop %r13
halt
.end