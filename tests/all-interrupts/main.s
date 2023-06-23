.extern handler
.global end, timer_counter, terminal_counter, term_out

.equ stack_pointer, 0xFFFFFF00
.equ term_out, 0xFFFFFF00
.equ tim_cfg, 0xFFFFFF10
.equ count5, 5
.equ count10, 10

.section main
	ld $0x00, %r1
	st %r1, tim_cfg
	ld $stack_pointer, %sp
  ld $handler, %r1
  csrwr %r1, %handler
wait:
	ld timer_counter, %r1
	ld $count5, %r2
	bne %r1, %r2, wait
	int
	st %r0, timer_counter
	ld $count10, %r2
	ld terminal_counter, %r1
	bgt %r2, %r1, wait
end:
	halt

.section data
timer_counter:
	.word 0
terminal_counter:
	.word 0
.end