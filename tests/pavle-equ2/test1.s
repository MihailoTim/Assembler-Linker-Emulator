.equ A, 5
.equ B, A + 1
.equ C, A + B - 4
.extern D
.equ E, D + C
.equ F, label1 - 3
.equ G, label1 - main
.equ H, label1 - main + E
.equ I, label1 + 3
.equ J, papa_main
.equ four, 0x04
.equ eight, 0x08
.global C
.extern end
.extern handler

.section papa_main
main:
  ld $handler, %r1
  csrwr %r1, %handler
label1:
  ld $data, %r1
  ld [%r1 + 0], %r2
  ld [%r1 + four], %r3
  ld [%r1 + eight], %r4
  ld [%r1 + 0xc], %r5
  ld [%r1 + 0x10], %r6
  ld [%r1 + 0x14], %r7
  ld [%r1 + 0x18], %r8
  ld [%r1 + 0x1c], %r9
  ld [%r1 + 0x20], %r11
  ld [%r1 + 0x24], %r12
  jmp end

.section data
.word A, B, C, D, E, F, G, H, I, J
.end