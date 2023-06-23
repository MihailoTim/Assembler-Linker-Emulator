.extern printer
.global isr_software
.section software_section
.equ software_message_len, sofrware_message_end-software_message_start
isr_software:
	push %r1
	push %r2

	ld $software_message_start, %r1
	ld $software_message_len, %r2
	push %r1
	push %r2
	call printer

	pop %r2
	pop %r1

	pop %r2
	pop %r1


	ret

.section messages
software_message_start:
	.ascii "SOFTWARE INTERRUPT"
sofrware_message_end:

.end