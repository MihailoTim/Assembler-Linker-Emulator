.extern handler
.global my_counter, end

.section main
	ld $0xFFFFFF00, %sp
  	ld $handler, %r1
  	csrwr %r1, %handler
wait:
	jmp wait

end:
	halt
.section data
my_counter:
	.word 0

.end