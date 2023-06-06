ASSEMBLER=../../asembler
LINKER=../../linker
EMULATOR=../../emulator

${ASSEMBLER} -o main.o main.s
${ASSEMBLER} -o a.o a.s
${ASSEMBLER} -o b.o b.s
${ASSEMBLER} -o c.o c.s
${ASSEMBLER} -o d.o d.s
${ASSEMBLER} -o e.o e.s
${LINKER} -relocatable \
  -o data.o \
  a.o b.o c.o d.o e.o
${LINKER} -hex \
  -place=main@0x40000000 \
  -o program.hex \
  main.o data.o
  
${EMULATOR} program.hex