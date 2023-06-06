.extern printer
.global isr_software
.section software_section
isr_software:
	push %r1
	push %r2

	ld $software_message, %r1
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
software_message:
	.ascii "SOFTWARE INTERRUPT"

.end