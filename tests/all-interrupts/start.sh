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
  -o reloc.o \
  handler.o terminal.o timer.o printer.o software.o
${LINKER} -hex \
  -place=main@0x40000000 \
  -o program.hex \
  main.o handler.o terminal.o timer.o printer.o software.o
${EMULATOR} program.hex