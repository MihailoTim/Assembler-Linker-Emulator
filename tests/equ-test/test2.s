.extern end, local_b, d
.global b, c, a
.equ b, d
.equ c, local_b + 2
.section test2
a:
  call end
.end