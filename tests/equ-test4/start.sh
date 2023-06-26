ASSEMBLER=../../asembler
LINKER=../../linker
EMULATOR=../../emulator

${ASSEMBLER} -o test1.o test1.s
${ASSEMBLER} -o test2.o test2.s
${ASSEMBLER} -o test3.o test3.s
${ASSEMBLER} -o test4.o test4.s
${LINKER} -relocatable \
  -o reloc.o \
  test1.o test2.o
${LINKER} -relocatable \
  -o reloc2.o \
  reloc.o test3.o
${LINKER} -hex \
  -place=text@0x40000000 \
  -o program.hex \
  reloc2.o  test4.o
${EMULATOR} program.hex