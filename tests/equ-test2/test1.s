.global A,B,C,D,J,K
.equ A, 1  # 1
.equ B, labela2 - labela1 # 5
.equ C, labela3 # 20
.equ D, C + 10 # 30
.equ J, labela3 - labela2 - labela1 + 10
.equ K, J - C + A # -4
.section text
.skip 5
labela1:
.skip 5
labela2:
.skip 15
labela3:
.end
