.global handler
.section my_code_handler
.equ a, 10
handler:
 push %r1
 push %r2
 csrrd %cause, %r1