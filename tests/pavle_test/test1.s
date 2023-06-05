.global my_counter
.section papa_data
.global inc_my_counter
my_counter:
 .word 0

.section papa_handler
inc_my_counter:
  ld my_counter, %r1
  ld $1, %r2
  add %r2, %r1
  st %r1, my_counter
  ret

.end