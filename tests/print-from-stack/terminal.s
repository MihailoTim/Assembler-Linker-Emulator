.extern printer
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

	pop %r2
	pop %r1

	ret

.section messages
terminal_message:
	.ascii "TERMINAL INTERRUPT"

.end