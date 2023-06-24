.equ x, 79 + a - b + y # 79
.equ y, a - papa_main # 4
.equ term_out , 0xFFFFFF00
.equ ascii_out, 84 + b - a - c + b  # 0x84
.extern papa_end1
.global ascii_out, term_out
.section papa_main
  ld $0x7, %r1
a:  
  csrwr %r1, %status
b:
  ld $0, %r2
c:
loop:
  ld $1, %r3
  st %r2, %r4
  and %r3, %r4
  ld $0, %r10
  beq %r10, %r4, even
odd:
  ld $x, %r4
  st %r4, term_out
  st %r4, %r9
  jmp finish
even:
  ld $ascii_out, %r4
#  halt
  st %r4, term_out
  st %r4, %r8
finish:
  ld $1, %r1
  add %r1, %r2
  ld $0x10, %r1
  bne %r1, %r2, loop
  jmp papa_end1

.end