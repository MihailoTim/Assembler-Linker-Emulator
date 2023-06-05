.extern inc_my_counter
.extern my_counter
.global handler1
.global handler2
.section papa_handler
handler1:
  push %r1
  push %r2
  csrrd %cause, %r1
  ld $3, %r2
  beq %r1, %r2, papa_terminal
  jmp finish
papa_terminal:
  call inc_my_counter
  ld $0x50000000 , %r1
  st %r1, 0xFFFFFF00
  jmp finish

handler2:
  push %r1
  push %r2
  # csrrd %cause, %r1
  # ld $2, %r2
  # beq %r1, %r2, papa_timer
  jmp papa_timer
papa_timer:
  call inc_my_counter
  ld $0x54000000 , %r1
  st %r1, 0xFFFFFF00
  jmp finish

finish:
  pop %r2
  pop %r1
  iret

.end