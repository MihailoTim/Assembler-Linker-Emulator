.extern handler
.global end

.section main
	ld $0x01, %r1
	st %r1, 0xFFFFFF10
	ld $0xFFFFFF00, %sp
  	ld $handler, %r1
  	csrwr %r1, %handler
wait:
	jmp wait
end:
	halt

.end