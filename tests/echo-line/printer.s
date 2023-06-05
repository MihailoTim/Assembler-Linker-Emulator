.extern message, my_counter
.global printer

.section printer_function
printer:
	push %r1
	push %r2
	push %r3
	push %r4
	ld $message, %r1
	ld my_counter, %r2
	add %r1, %r2
	ld $0, %r3
	add %r1, %r3
loop:
	ld [%r3], %r4
	st %r4, 0xFFFFFF00
	ld $1, %r1
	add %r1, %r3
	bne %r3, %r2, loop

	ld $0, %r1
	st %r1, my_counter

	pop %r4
	pop %r3
	pop %r2
	pop %r1
	ret

.end