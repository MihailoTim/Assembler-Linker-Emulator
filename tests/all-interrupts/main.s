.extern handler
.global end, counter

.section main
	ld $0x00, %r1
	st %r1, 0xFFFFFF10
	ld $0xFFFFFF00, %sp
  	ld $handler, %r1
  	csrwr %r1, %handler
wait:
	ld counter, %r1
	ld $5, %r2
	bne %r1, %r2, wait
	int
	st %r0, counter
	jmp wait
end:
	halt

.section data
counter:
	.word 0
.end