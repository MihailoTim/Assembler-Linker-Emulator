.extern printer, timer_counter
.global isr_timer
.section timer_section
isr_timer:

	push %r1
	push %r2

	ld $timer_message, %r1
	ld $15, %r2
	push %r1
	push %r2
	call printer

	pop %r2
	pop %r1

	ld timer_counter, %r1
	ld $1, %r2
	add %r2, %r1
	st %r1, timer_counter

	pop %r2
	pop %r1


	ret

.section messages
timer_message:
	.ascii "TIMER INTERRUPT"

.end