ASSEMBLER=../../asembler
LINKER=../../linker
EMULATOR=../../emulator

${ASSEMBLER} -o main.o main.s
${ASSEMBLER} -o handler.o handler.s
${ASSEMBLER} -o timer.o timer.s
${ASSEMBLER} -o terminal.o terminal.s
${ASSEMBLER} -o software.o software.s
${LINKER} -relocatable \
  -o reloc.o \
  handler.o terminal.o timer.o software.o
${LINKER} -hex \
  -place=main@0x40000000 \
  -o program.hex \
  reloc.o main.o
${EMULATOR} program.hex