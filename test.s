ld $0, %r1
csrwr %r1, %status
wait_for_click:
ld 0xFFFFFF04, %r1
beq %r1, %r0, wait_for_click
echo:
st %r1, 0xFFFFFF00
st %r0, 0xFFFFFF04
jmp wait_for_click
halt
.end