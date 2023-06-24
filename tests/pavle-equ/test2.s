.extern papa_end2, ascii_out, term_out
.global papa_end1
.section papa_main
papa_end1:
  ld $ascii_out, %r7
  st %r7, term_out
  jmp papa_end2

.end