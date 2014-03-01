ISA-doge
========

////////////////////////////   OVERVIEW   /////////////////////////////////////

This project was adapted from an exam question for EECS 370 at uMich. The question
created an Instruction Set Architecture called ISA-doge. This architecture has four
registers, and runs on a 16 bit machine. I have written an assembler and simulator
for this ISA.

assemble - takes in a file written in ISA-doge and converts it to machine code

simulate - takes the machine code and simulates it while showing the registers
and memory at each step



//////////////////////////   Directions   ////////////////////////////////////

1. write your code in ISA doge into a file. Lets call it test.doge
2. run ./assemble test.doge test.mc to convert your doge to machine code
3. run ./simulate test.mc to simulate your code

Notes:
  - error checking is imperfect, so make sure your code is valid
  - to simulate memory, use .fill at the TOP of your program
  - link to the memory w immidiates that you calculate youself
  


////////////////////////   ISA-doge   //////////////////////////////////

Opcodes: wow, such, add, sub, so, very, much, lvl

type 1 (wow, lvl): 
 - bits 15 - 13: opcode
 - bits 12 - 0: unused, all zero
  
type 2 (so, very, such):
 - bits 15 - 13: opcode
 - bit 12: unused, zero
 - bits 11 - 0: immediate value
  
type 3 (much):
 - bits 15 - 13: opcode
 - bits 12 - 3: unused, all zero
 - bits 2 - 0: register
  
type 4 (add, sub):
 - bits 15 - 13: opcode
 - bit 12: 1 is arg is an immediate, 0 if arg is a register
 - bits 11 - 0: either immediate value or register
  
  
wow, 000:
 - used as label for the jumps that such makes

lvl, 111:
 - halts program

such <immediate>, 001:
 - compares value in accumulator with value immediate in memory,
   jumps to wow accordingly
 - EXAMPLE:
    - wow       // such jumps here if ACC < MEM[5]
    - ...
    - such #5
    - lvl       // runs here if ACC == MEM[5]
    - ...
    - wow       // sucj jumos here if ACC > MEM[5]

so <immediate>, 100:
 - loads value of immediate location into accumulator
 - Ex, so #5  //  ACC = MEM[5]

very <immediate>, 101:
 - saves value of accumulator to immediate location
 - Ex, very #5  //  MEM[5] = ACC

much <register>, 110:
 - swaps value in acculmulator with value in register

add <reg or imd>, 010:
 - adds immediate or register to accumulator

sub <reg or imd>, 011:
 - subtracts immediate or register from accumulator






