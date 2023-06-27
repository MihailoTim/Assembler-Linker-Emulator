.extern F
.global A,B,C,D,J,K,N
.equ A, 1  # 1
.equ B, labela2 - labela1 # 5
.equ C, labela3 + 5 # 30
.equ D, C + 10 # 40
.equ J, labela3 - labela2 - labela1 + 10 # 20
.equ K, J - C + A # -9
.equ M, labela2 + 7
.equ N, F + M - C # F - 8
.section text
.skip 5
labela1:
.skip 5
labela2:
.skip 15
labela3:
.end
