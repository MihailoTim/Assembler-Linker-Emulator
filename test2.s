.extern end, calla, local_b
.global b, c, a
.equ b, 1
.equ c, local_b
.section test2
a:
  call end
.end