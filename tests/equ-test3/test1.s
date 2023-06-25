.equ A, 1
.equ B, data 
.section main
ld $0, %r1
ld [%r1 + A], %r2
halt
.skip 8
.section data
  .word 1,2,3,4
.end