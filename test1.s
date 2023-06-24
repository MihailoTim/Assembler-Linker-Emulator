.extern a,b,c
.global calla, end


.equ local_b, b

.section test1

.equ local_c, c + 7
calla:
  ld $local_b, %r2
  ld $local_c, %r3
  call a
end:
  halt
.end