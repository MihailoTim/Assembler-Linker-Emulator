.global printer
.extern term_out
.equ LF, 0xFFFFFF00 - term_out + 0x0A

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
	st %r4, term_out
	ld $1, %r1
	add %r1, %r3
	bne %r3, %r2, loop

	ld $0x0D, %r1
	st %r1, term_out
	ld $LF, %r1
	st %r1, term_out

	pop %r4
	pop %r3
	pop %r2
	pop %r1

	ret

.end