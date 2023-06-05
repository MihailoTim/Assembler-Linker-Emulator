ASSEMBLER=../../asembler
LINKER=../../linker
EMULATOR=../../emulator

${ASSEMBLER} -o test1.o test1.s
${ASSEMBLER} -o test2.o test2.s
${ASSEMBLER} -o test3.o test3.s
${LINKER} -hex \
  -place=papa_main@0x40000000 \
  -o program.hex \
  test1.o test2.o test3.o
${EMULATOR} program.hex