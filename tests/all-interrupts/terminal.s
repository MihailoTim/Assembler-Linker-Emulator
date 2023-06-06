.extern printer, terminal_counter
.global isr_terminal
.section terminal_section

isr_terminal:
	push %r1
	push %r2

	ld $terminal_message, %r1
	ld $18, %r2
	push %r1
	push %r2
	call printer

	pop %r2
	pop %r1

	ld terminal_counter, %r1
	ld $1, %r2
	add %r2, %r1
	st %r1, terminal_counter

	pop %r2
	pop %r1

	ret

.section messages
terminal_message:
	.ascii "TERMINAL INTERRUPT"

.end