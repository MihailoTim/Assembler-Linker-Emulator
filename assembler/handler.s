.global symbol1, sybmol2,    symbol3
labela : .extern symbol4, symbol5, symbol6
.section text
.ascii "this is some string"
labela3 :
.word a, b, c, d, e
.section data
.skip 13
labela2 :
.section bss
.end