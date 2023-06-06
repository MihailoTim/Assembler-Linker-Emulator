.extern handler
.global iterations_left, end, counter

.section main
	ld $0xFFFFFF00, %sp
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