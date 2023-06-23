# file: main.s
.extern handler
.global my_start, my_counter
.equ count, 5
.equ stack_pointer, 0xFFFFFEFE
.equ term_out, 0xFFFFFF10

.section code
my_start:
    ld $stack_pointer, %sp
    ld $handler, %r1
    csrwr %r1, %handler

    ld $0x1, %r1
    st %r1, term_out
wait:
    ld my_counter, %r1
    ld $count, %r2
    bne %r1, %r2, wait
    halt

.section my_data
my_counter:
.word 0

.end
