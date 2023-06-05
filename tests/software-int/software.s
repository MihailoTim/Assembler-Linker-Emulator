.global isr_software
.section software_section
isr_software:
	ld $0x03, %r5
	ret

.end