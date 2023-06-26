.extern printer, terminal_counter
.global isr_terminal
.section terminal_section

.equ terminal_message_len, terminal_message_end-terminal_message_start

isr_terminal:
	push %r1
	push %r2

	ld $terminal_message_start, %r1
	ld $terminal_message_len, %r2
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
terminal_message_start:
	.ascii "TERMINAL INTERRUPT"
terminal_message_end:

.end