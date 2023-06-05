ASSEMBLER=./asembler
LINKER=./linker
EMULATOR=./emulator

${ASSEMBLER} -o main.o main.s
${ASSEMBLER} -o handler.o handler.s
${ASSEMBLER} -o isr_terminal.o isr_terminal.s
${ASSEMBLER} -o isr_timer.o isr_timer.s
${LINKER} -hex\
  -place=my_code@0x40000000 \
  -place=my_handler@0x50 \
  -o program.hex \
  isr_terminal.o isr_timer.o handler.o main.o
${EMULATOR} program.hex