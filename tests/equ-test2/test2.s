.extern A,B,C,D,K
.global E,F,G,H,L
.equ E, labela6 - labela5 # 15
.equ F, labela5 # 5
.equ G, C + 10 # 40
.equ H, D - 3 # 37
.equ L, K + 2 # -7
.section data
labela4:
.skip 5
labela5:
.skip 15
labela6:
.end