.global real_start
.extern sym_addr
.extern get_back
.section papa_main
real_start:
   ld sym_addr, %r2
   ld $0, %r4
for:
   ld $0x70000000, %r3
   st %r3, 0xFFFFFF00
   st %r3, [%r2]
   ld $1, %r3
   add %r3, %r2
   add %r3, %r4
   ld $6, %r3
   bne %r3, %r4, for
   jmp get_back

.end
