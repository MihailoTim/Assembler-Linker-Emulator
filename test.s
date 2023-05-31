jmp init
.skip 20
init:
not %r1 # r1 = -1
sub %r1, %r2 # r2 = 1
add %r2, %r3 # r3 = 1
shl %r3, %r3 # r3 = 2
push %r3
call function
pop %r6
halt
function:
not %r7 # r1 = -1
sub %r7, %r8 # r2 = 1
add %r8, %r9 # r3 = 1
shl %r9, %r9 # r3 = 2
ret
.end