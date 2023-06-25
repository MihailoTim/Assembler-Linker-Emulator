ASSEMBLER=../../asembler
LINKER=../../linker
EMULATOR=../../emulator

${ASSEMBLER} -o test1.o test1.s
${LINKER} -hex \
  -place=main@0x40000000 \
  -o program.hex \
  test1.o
${EMULATOR} program.hex