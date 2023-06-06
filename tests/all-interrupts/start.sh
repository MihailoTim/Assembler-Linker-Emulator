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
  -o interrupt_routines.o \
  terminal.o timer.o software.o
${LINKER} -relocatable \
  -o code.o \
  main.o handler.o printer.o
${LINKER} -hex \
  -place=main@0x40000000 \
  -o program.hex \
  code.o interrupt_routines.o
  
${EMULATOR} program.hex