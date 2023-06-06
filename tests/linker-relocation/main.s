.extern a, b, c, d, e

.section main
	ld a, %r1
	ld b, %r2
	ld c, %r3
	ld d, %r4
	ld e, %r5
	halt

.end