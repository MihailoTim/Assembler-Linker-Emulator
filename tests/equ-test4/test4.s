.extern equ_data, B, data2
.section text
ld B, %r1
ld $data2, %r2
ld [%r2 + 0x0], %r3
ld [%r2 + 0x4], %r4
ld $equ_data, %r5
ld equ_data, %r6
halt
.end