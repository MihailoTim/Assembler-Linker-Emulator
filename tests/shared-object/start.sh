ASSEMBLER=../../asembler
LINKER=../../linker
EMULATOR=../../emulator

${ASSEMBLER} -o main.o main.s
${ASSEMBLER} -o handler1.o handler1.s
${ASSEMBLER} -o handler2.o handler2.s
${ASSEMBLER} -o timer.o timer.s
${ASSEMBLER} -o terminal.o terminal.s
${ASSEMBLER} -o software.o software.s
${LINKER} -relocatable \
  -o notHandler.o \
  main.o terminal.o software.o timer.o
${LINKER} -hex \
  -place=main@0x40000000 \
  -o program1.hex \
  notHandler.o handler1.o
${LINKER} -hex \
  -place=main@0x40000000 \
  -o program2.hex \
  notHandler.o handler2.o