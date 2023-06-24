.global papa_end2
.extern ascii_out, term_out
.section papa_main
papa_end2:
  ld $ascii_out, %r7
  st %r7, term_out
  halt

.end