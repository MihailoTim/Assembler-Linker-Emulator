.extern handler
.global iterations_left, end, counter, term_out, tim_cfg

.equ stack_pointer,0xFFFFFF00
.equ term_out, 0xFFFFFF00
.equ tim_cfg, 0xFFFFFF10

.section main
	ld $stack_pointer, %sp
  ld $handler, %r1
  csrwr %r1, %handler
wait:
	jmp wait
end:
	halt
.section data
iterations_left:
	.word 10
counter:
	.word 0

.end