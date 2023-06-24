ASSEMBLER=../../asembler
LINKER=../../linker
EMULATOR=../../emulator

${ASSEMBLER} -o test1.o test1.s
${ASSEMBLER} -o test2.o test2.s
${LINKER} -hex \
  -place=test1@0x40000000 \
  -o program.hex \
  test1.o test2.o
${EMULATOR} program.hex