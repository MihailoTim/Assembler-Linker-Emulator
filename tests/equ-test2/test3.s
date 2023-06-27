.extern A,B,C,D,E,F,G,H,J,K,L,N
.equ I, G - 7 # 33
.section main
ld $A, %r1
ld $B, %r2
ld $C, %r3
ld $D, %r4
ld $E, %r5
ld $F, %r6
ld $G, %r7
ld $H, %r8
ld $I, %r9
ld $J, %r10
ld $K, %r11
ld $L, %r12
ld $N, %r13
halt
.end