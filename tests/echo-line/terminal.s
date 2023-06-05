.extern call_printer, message, my_counter
.global isr_terminal, return_from_call
.section terminal_section

isr_terminal:
	push %r1
	push %r2
	push %r3
	push %r4

	ld $message, %r4
	ld 0xFFFFFF04, %r1
    st %r1, 0xFFFFFF00
	ld my_counter, %r3
	add %r3, %r4
	st %r1, [%r4]
	ld $1, %r4
	add %r4, %r3
	st %r3, my_counter
	ld $0x0A, %r2
	beq %r1, %r2, call_printer
	ld $20, %r4
	beq %r3, %r4, call_printer

return_from_call:
	pop %r4
	pop %r3
	pop %r2
	pop %r1

	ret

.end