# file: main.s
.extern handler1
.extern handler2
.extern my_counter
.section papa_main
  ld $0xFFFFFF00, %sp
  ld $handler1, %r1
  csrwr %r1, %handler
  ld $0x1, %r1
  st %r1, 0xFFFFFF10
wait:
  ld my_counter, %r1
  ld $3, %r2
  bne %r1, %r2, wait

  ld $handler2, %r1
  csrwr %r1, %handler

wait2:
  ld my_counter, %r1
  ld $6, %r2
  bne %r1, %r2, wait2

  halt

.end