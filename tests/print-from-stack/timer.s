.extern printer
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

	pop %r2
	pop %r1


	ret

.section messages
timer_message:
	.ascii "TIMER INTERRUPT"

.end