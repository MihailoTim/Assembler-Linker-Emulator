ASSEMBLER=./asembler
LINKER=./linker
EMULATOR=./emulator

${ASSEMBLER} -o equ.o equ.s
${ASSEMBLER} -o equ2.o equ2.s
${LINKER} -hex \
  -o program.hex \
  equ.o equ2.o
${EMULATOR} program.hex