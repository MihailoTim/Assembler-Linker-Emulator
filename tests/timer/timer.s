.extern iterations_left, end, counter, term_out, tim_cfg
.global isr_timer
.section timer_section
isr_timer:
	push %r1
	push %r2
	push %r3
	ld iterations_left, %r1
	beq %r0, %r1, end
	ld $0x54, %r2
    st %r2, term_out
	ld counter, %r3
	ld $0x01, %r2
	add %r2, %r3
	st %r3, counter
	bne %r1, %r3, skip
	ld $0x0, %r1
	st %r1, counter
	sub %r2, %r3
	st %r3, iterations_left
	ld $0xD, %r2
  st %r2, term_out
	ld $0xA, %r2
  st %r2, term_out
	ld tim_cfg, %r1
	ld $0x01, %r2
	add %r2, %r1
	st %r1, tim_cfg
skip:
	pop %r3
	pop %r2
	pop %r1
	ret

.end