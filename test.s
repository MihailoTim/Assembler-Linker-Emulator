ld $0x18, %r2
csrwr %r2, %handler
wait_for_click:
ld 0xFFFFFF04, %r1
jmp wait_for_click
halt
handler:
csrwr %r0, %cause
 push %r1
 push %r2
 csrrd %cause, %r1
 ld $3, %r2
 beq %r1, %r2, my_isr_terminal
# obrada prekida od terminala
my_isr_terminal:
 ld 0xFFFFFF04, %r1
 st %r1, 0xFFFFFF00
finish:
 pop %r2
 pop %r1
 iret #komentar
 # .emd
 halt
 .end