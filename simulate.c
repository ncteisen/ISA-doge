/*
 * Instruction-level simulator for the LC
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMMEMORY 4096 /* maximum number of words in memory */
#define NUMREGS 4 /* number of machine registers */
#define MAXLINELENGTH 1000

#define WOW 0
#define SUCH 1
#define ADD 2
#define SUB 3
#define SO 4
#define VERY 5
#define MUCH 6
#define LVL 7

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
    int acc;
} stateType;

void printState(stateType *);
void run(stateType);
int convertNum(int);

int
main(int argc, char *argv[])
{
    int i;
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;
    
    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }
    
    /* initialize memories and registers */
    for (i = 0; i < NUMMEMORY; i++) {
        state.mem[i] = 0;
    }
    for (i = 0; i < NUMREGS; i++) {
        state.reg[i] = 0;
    }
    
    state.pc = 0;
    state.acc = 0;
    
    /* read machine-code file into instruction/data memory (starting at
     address 0) */
    
    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s\n", argv[1]);
        perror("fopen");
        exit(1);
    }
    
    for (state.numMemory=0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
         state.numMemory++) {
        if (state.numMemory >= NUMMEMORY) {
            printf("exceeded memory size\n");
            exit(1);
        }
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }
    
    printf("\n");
    
    /* run never returns */
    run(state);
    
    return(0);
}

void
run(stateType state)
{
    int arg, addressField, imm, temp;
    int instructions=0;
    int opcode;
    int maxMem = -1;	/* highest memory address touched during run */
    
    for (; 1; instructions++) { /* infinite loop, exits when it executes halt */
        printState(&state);
        
        if (state.pc < 0 || state.pc >= NUMMEMORY) {
            printf("pc went out of the memory range\n");
            exit(1);
        }
        
        maxMem = (state.pc > maxMem)?state.pc:maxMem;
        
        /* this is to make the following code easier to read */
        opcode = state.mem[state.pc] >> 13;
        
        arg = state.mem[state.pc] & 0x7;
        addressField = state.mem[state.pc] & 0xFFF;
        imm = (state.mem[state.pc] >> 12) & 1;
        state.pc++;
        
        if (opcode == WOW) {
        } else if (opcode == SUCH) {
            if (state.acc < state.mem[addressField]) {
                state.pc--;
                while (state.pc >= 0) {
                    if (state.mem[state.pc] == 0)
                        break;
                    state.pc--;
                }
                if (state.pc == -1) {
                    printf("such found no matching wow\n");
                    exit(1);
                }
            } else if (state.acc > state.mem[addressField]) {
                state.pc++;
                while (state.pc < NUMMEMORY) {
                    if (state.mem[state.pc] == 0)
                        break;
                    state.pc++;
                }
                if (state.pc == NUMMEMORY) {
                    printf("such found no matching wow\n");
                    exit(1);
                }
            } else { }
        } else if (opcode == ADD) {
            if (imm) {
                state.acc += addressField;
            } else {
                state.acc += state.reg[arg];
            }
        }  else if (opcode == SUB) {
            if (imm) {
                state.acc -= addressField;
            } else {
                state.acc -= state.reg[arg];
            }
        } else if (opcode == SO) {
            state.acc = state.mem[addressField];
        } else if (opcode == VERY) {
            state.mem[addressField] = state.acc;
        } else if (opcode == MUCH) {
            temp = state.acc;
            state.acc = state.reg[arg];
            state.reg[arg] = temp;
        } else if (opcode == LVL) {
            printf("machine halted\n");
            printf("total of %d instructions executed\n", instructions+1);
            printf("final state of machine:\n");
            printState(&state);
            exit(0);
        } else {
            printf("error: illegal opcode 0x%x\n", opcode);
            exit(1);
        }
        state.reg[0] = 0;
    }
}

void
printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
	for (i=0; i<statePtr->numMemory; i++) {
	    printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}
    printf("\tregisters:\n");
	for (i=0; i<NUMREGS; i++) {
	    printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
    printf("\tAccumulator: %d\n", statePtr->acc);
    printf("end state\n");
}

int
convertNum(int num)
{
    /* convert a 12-bit number into a 16-bit Sun integer */
    if (num & (1<<11) ) {
        num -= (1<<12);
    }
    return(num);
}