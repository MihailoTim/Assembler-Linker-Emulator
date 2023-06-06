.global isr_timer
.section timer_section
isr_timer:
	ld $0x61, %r1
    st %r1, 0xFFFFFF00
	ret

.end