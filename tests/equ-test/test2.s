.extern end, local_b, d, local_c
.global b, c, a
.equ b, d
.equ c, b + 2
.section test2
a:
  ld $local_c, %r1
  call end
.end