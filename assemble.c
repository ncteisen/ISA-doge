/* Assembler for LC */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAXLINELENGTH 1000
#define MAXNUMLABELS 65536
#define MAXLABELLENGTH 7 /* includes the null character termination */

#define WOW 0
#define SUCH 1
#define ADD 2
#define SUB 3
#define SO 4
#define VERY 5
#define MUCH 6
#define LVL 7

int readAndParse(FILE *, char *, char *);
int isNumber(char *);
void testRegArg(char *);
void testAddrArg(char *);

int
main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    int address;
    char arg[MAXLINELENGTH], opcode[MAXLINELENGTH];
    int i;
    int numLabels=0;
    int num;
    int addressField;
    
    char labelArray[MAXNUMLABELS][MAXLABELLENGTH];
    int labelAddress[MAXNUMLABELS];
    
    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
               argv[0]);
        exit(1);
    }
    
    inFileString = argv[1];
    outFileString = argv[2];
    
    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }
    
    /* map symbols to addresses */
    
    /* assume address start at 0 */
    for (address=0; readAndParse(inFilePtr, opcode, arg);
         address++) {
        /*
         printf("%d: label=%s, opcode=%s, arg0=%s, arg1=%s, arg2=%s\n",
         address, label, opcode, arg0, arg1, arg2);
         */
        
        /* check for illegal opcode */
        if (strcmp(opcode, "wow") && strcmp(opcode, "such") &&
	        strcmp(opcode, "add") && strcmp(opcode, "sub") &&
            strcmp(opcode, "so") && strcmp(opcode, "very") &&
            strcmp(opcode, "much") && strcmp(opcode, "lvl") &&
            strcmp(opcode, ".fill")) {
            printf("error: unrecognized opcode %s at address %d\n", opcode,
                   address);
            exit(1);
        }
        
        /* check register fields */
        if (!strcmp(opcode, "much")) {
            testRegArg(arg);
        }
        
        if (!strcmp(opcode, "such") || !strcmp(opcode, "so")
            || !strcmp(opcode, "very") ) {
            testAddrArg(arg);
        }
        
    }
    
    /* now do second pass (print machine code, with symbols filled in as
     addresses) */
    rewind(inFilePtr);
    for (address=0; readAndParse(inFilePtr, opcode, arg);
         address++) {
        if (!strcmp(opcode, "wow")) {
            num = (WOW << 13);
        } else if (!strcmp(opcode, "such")) {
            arg[0] = '0';
            num = (SUCH << 13) | (atoi(arg));
        } else if (!strcmp(opcode, "very")) {
            arg[0] = '0';
            num = (VERY << 13) | (atoi(arg));
        } else if (!strcmp(opcode, "much")) {
            num = (MUCH << 13) | (atoi(arg));
        } else if (!strcmp(opcode, "so")) {
            arg[0] = '0';
            num = (SO << 13 | (atoi(arg)));
        } else if (!strcmp(opcode, "lvl")) {
            num = (LVL << 13);
        } else if (!strcmp(opcode, "add")) {
            if (arg[0] == '#') {
                arg[0] = '0';
                num = (ADD << 13 | 1 << 12 | atoi(arg));
            } else {
                num = (ADD << 13 | atoi(arg));
            }
        } else if (!strcmp(opcode, "sub")) {
            if (arg[0] == '#') {
                arg[0] = '0';
                num = (SUB << 13 | 1 << 12 | atoi(arg));
            } else {
                num = (SUB << 13 | atoi(arg));
            }
        } else if (!strcmp(opcode, ".fill")) {
            num = atoi(arg);
        }
        /* printf("(address %d): %d (hex 0x%x)\n", address, num, num); */
        fprintf(outFilePtr, "%d\n", num);
    }
    
    exit(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *opcode, char *arg)
{
    char line[MAXLINELENGTH];
    char *ptr = line;
    
    /* delete prior values */
    arg[0] = opcode[0] = '\0';
    
    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
        /* reached end of file */
        return(0);
    }
    
    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
        printf("error: line too long\n");
        exit(1);
    }
    
    
    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
           opcode, arg);
    return(1);
}

int
isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}

/*
 * Test register argument; make sure it's in range and has no bad characters.
 */
void
testRegArg(char *arg)
{
    int num;
    char c;
    
    if (atoi(arg) < 0 || atoi(arg) > 7) {
        printf("error: register out of range\n");
        exit(2);
    }
    if (sscanf(arg, "%d%c", &num, &c) != 1) {
        printf("bad character in register argument\n");
        exit(2);
    }
}

/*
 * Test addressField argument.
 */
void
testAddrArg(char *arg)
{
    int num;
    char c;
    arg[0] = '0';
    /* test numeric addressField */
    if (isNumber(arg)) {
        if (sscanf(arg, "%d%c", &num, &c) != 1) {
            printf("bad character in addressField\n");
            exit(2);
        }
    }
}