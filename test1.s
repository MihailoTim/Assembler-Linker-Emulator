.equ A, 1
.equ B, data 
.equ C, main + 17
.section main
ld $0, %r1
ld [%r1 + A], %r2
ld $C, %r3
halt
.skip 8
.section data
  .word 1,2,3,4
.end