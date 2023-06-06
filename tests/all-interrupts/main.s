.extern handler
.global end, timer_counter, terminal_counter

.section main
	ld $0x00, %r1
	st %r1, 0xFFFFFF10
	ld $0xFFFFFF00, %sp
  	ld $handler, %r1
  	csrwr %r1, %handler
wait:
	ld timer_counter, %r1
	ld $5, %r2
	bne %r1, %r2, wait
	int
	st %r0, timer_counter
	ld $10, %r2
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