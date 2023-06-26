.extern equ_data, B, data2
.section text
.equ zero, 0x00
.equ four, 0x04
ld B, %r1
ld $data2, %r2
ld [%r2 + zero], %r3
ld [%r2 + four], %r4
ld $equ_data, %r5
ld equ_data, %r6
halt
.end