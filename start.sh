ASSEMBLER=./asembler
LINKER=./linker
EMULATOR=./emulator

${ASSEMBLER} -o main.o main.s
${ASSEMBLER} -o handler.o handler.s
${ASSEMBLER} -o isr_terminal.o isr_terminal.s
${ASSEMBLER} -o isr_timer.o isr_timer.s
${LINKER} -hex \
  -place=my_code@0x40000000 \
  -place=my_data@0x30000004 \
  -place=my_handler@0x10000014 \
  -o program.hex \
  main.o isr_terminal.o isr_timer.o handler.o
${EMULATOR} output.hex