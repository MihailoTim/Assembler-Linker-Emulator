.extern g1

.equ x2, x + 17
.equ x, 3 + a - -5
.equ y, 4
.equ c, a
.equ d, b - a + 7
.equ e, g1
.section text
.skip 10
a:
.skip 20
b:
call x
call x2
call y
call c
call d
call e
.end