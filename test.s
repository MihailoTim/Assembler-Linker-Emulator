jmp init
.skip 20
init:
not %r1 # r1 = -1
sub %r1, %r2 # r2 = 1
add %r2, %r3 # r3 = 1
add %r2, %r4 
add %r0, %r5
shl %r3, %r3 # r3 = 2
shl %r3, %r3 # r3 = 8
start:
sub %r2, %r3
add %r4, %r5
beq %r3, %r0, end
int
end:
halt
.end