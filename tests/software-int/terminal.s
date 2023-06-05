.extern my_counter, end
.global isr_terminal
.section terminal_section

isr_terminal:
	push %r1
	push %r2
	push %r3
	push %r4

	ld 0xFFFFFF04, %r1
    st %r1, 0xFFFFFF00
	ld my_counter, %r3
	ld $1, %r4
	add %r4, %r3
	st %r3, my_counter
	ld $20, %r4
	beq %r3, %r4, end

	pop %r4
	pop %r3
	pop %r2
	pop %r1

	ret

.end