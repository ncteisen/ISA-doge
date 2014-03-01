ISA-doge
========

////////////////////////////   OVERVIEW   /////////////////////////////////////

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

_________________________________________________________________________
| wow              |                                                     |
| ...              |  'such' - if ACC > MEM[imd], then jump to next wow  |
| such <immediate> |           if ACC < MEM[imd], then jump to prev wow  |
| ...              |           if ACC == MEM[imd], then do nothing       |
| wow              |                                                     |
|__________________|_____________________________________________________|









