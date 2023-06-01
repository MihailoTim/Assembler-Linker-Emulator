ld $0x18, %r2
csrwr %r2, %handler
wait_for_click:
ld 0xFFFFFF04, %r1
jmp wait_for_click
halt
handler:
 push %r1
 push %r2
 csrrd %cause, %r1
 ld $3, %r2
 bne %r1, %r2, finish
 call my_isr_terminal
# obrada prekida od terminala
finish:
 pop %r2
 pop %r1
 csrwr %r0, %cause
 iret #komentar
 my_isr_terminal:
 ld 0xFFFFFF04, %r1
 st %r1, 0xFFFFFF00
 ret
 .end