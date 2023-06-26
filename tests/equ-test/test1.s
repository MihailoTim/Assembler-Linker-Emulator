.extern a,b,c
.global calla, end, local_b, d


.equ local_b, b + 3
.equ d, c + 4

.section test1

.equ local_c, c + 7
calla:
  ld $local_b, %r2
  ld $local_c, %r3
  ld $d, %r4
  call a
end:
  halt
.end