.global sym_addr
.section papa_data
sym_addr: .word arr
.skip 0x6
arr: .word 0x00,0x00,0x00,0x00

.end