.global printer

.section printer_function
printer:
	push %r1
	ld [%sp + 12], %r1 # string start address
	push %r2
	ld [%sp + 12], %r2 # string length
	push %r3
	push %r4
	add %r1, %r2
	ld $0, %r3
	add %r1, %r3
loop:
	ld [%r3], %r4
	st %r4, 0xFFFFFF00
	ld $1, %r1
	add %r1, %r3
	bne %r3, %r2, loop

	ld $0xD, %r1
	st %r1, 0xFFFFFF00
	ld $0xA, %r1
	st %r1, 0xFFFFFF00

	pop %r4
	pop %r3
	pop %r2
	pop %r1

	ret

.end