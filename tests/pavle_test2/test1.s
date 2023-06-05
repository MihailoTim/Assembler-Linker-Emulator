.extern real_start
.extern sym_addr
.global get_back
.section papa_main
  ld $0xFFFFFF00, %sp
  ld $handler, %r1
  csrwr %r1, %handler
  jmp real_start
get_back:
  ld sym_addr, %r2
#  ld $2, %r4
#  sub %r4, %r2
  st %r2, %r8
  ld $0, %r4
for:
  ld [%r2], %r3
  ld $0xFF000000, %r1
  and %r1, %r3
  st %r3, %r9
  st %r3, 0xFFFFFF00
  ld $1, %r3
  add %r3, %r2
  add %r3, %r4
  ld $6, %r3
  bne %r3, %r4, for

  halt

handler:
  push %r1
  ld $0x50000000, %r1
  st %r1, 0xFFFFFF00
  pop %r1
  iret

.end