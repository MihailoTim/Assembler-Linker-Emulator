.extern handler, return_from_call, printer
.global message, call_printer, my_counter

.section main
	ld $0xFFFFFF00, %sp
  	ld $handler, %r1
  	csrwr %r1, %handler
wait:
	jmp wait

call_printer:
	call printer
	jmp return_from_call
.section data
my_counter:
	.word 0
message:
.skip 100

.end