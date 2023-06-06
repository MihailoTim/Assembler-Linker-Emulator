ASSEMBLER=../../asembler
LINKER=../../linker
EMULATOR=../../emulator

${ASSEMBLER} -o main.o main.s
${ASSEMBLER} -o handler.o handler.s
${ASSEMBLER} -o timer.o timer.s
${ASSEMBLER} -o terminal.o terminal.s
${ASSEMBLER} -o software.o software.s
${ASSEMBLER} -o printer.o printer.s
${LINKER} -relocatable \
  -o reloc1.o \
  main.o printer.o software.o
${LINKER} -relocatable \
  -o reloc2.o \
  handler.o terminal.o timer.o
${LINKER} -hex \
  -place=main@0x40000000 \
  -o program.hex \
  reloc1.o reloc2.o
${EMULATOR} program.hex