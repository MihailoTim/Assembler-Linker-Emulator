# file: isr_terminal.s

.extern my_counter

.section isr
# prekidna rutina za terminal
.global isr_terminal
isr_terminal:
    push %r1
    push %r2
    ld 0xFFFFFF04, %r1
    ld $2, %r2
    add %r2, %r1
    st %r1, 0xFFFFFF00
    ld my_counter, %r1
    ld $1, %r2
    add %r2, %r1
    st %r1, my_counter
    pop %r2
    pop %r1
    ret
  
.end
