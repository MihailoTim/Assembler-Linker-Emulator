.equ ascii_out, 80
.equ term_out, 0xFFFFFF00
.global end
.section papa_end
end:
  ld $ascii_out, %r10
  st %r10, term_out
  halt

.end