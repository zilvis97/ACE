/**************************************************************************
 * Assessment Title: ACE3
 *
 *
 * Number of Submitted C Files: 2
 *
 *
 * Date: 2018-11-24
 *
 *
 * Authors:
 *	1. Zilvinas Skuodys, Reg no: 201710925
 *	2. Jack Dale, Reg no: 201707564
 *	3. Mohammed Adil Ali Akbar, Reg no: 201708796
 *	4. Paul Power, Reg no: 201707831
 *
 *
 *	Statement: We confirm that this submission is all our own work.
 *
 *  (Signed)_____________________________________(Zilvinas Skuodys)
 *
 * 	(Signed)_____________________________________(Jack Dale)
 *
 *	(Signed)_____________________________________(Mohammed Adil Ali Akbar)
 *
 *	(Signed)_____________________________________(Paul Power)
 *
 **************************************************************************/

#ifndef ACE3_H_INCLUDED
#define ACE3_H_INCLUDED

#define SIZE 4096
#define INSTR_LENGTH 16

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    int mem[SIZE];
	int ac_reg;
	int instr_reg;
	int mbr;
	int pc;
	int mar;
	int in_reg;
	int out_reg;
	int status_flag;
} ProgramRegisters;

//load memory functions
void loadDefaultMemory(ProgramRegisters *pr);
int manualInput(ProgramRegisters *pr);
int fileInput(char *filename, ProgramRegisters *pr);

//conversion and print functions
void printOperand(int dec);
void printAssemblyInstructions(ProgramRegisters pr);
void printNonEmptyMemory(ProgramRegisters pr);
char *getOperation(int code);
int toDec(char *operand);
int toDecimalFromBinary(long long n);
int twosComplement(int dec);
int toDecFromComplement(int compl);
long long toBinary(int dec, int opcode_true);

//ACE3 computer functions
void add(ProgramRegisters *pr);
void sub(ProgramRegisters *pr);
void load(ProgramRegisters *pr);
void store(ProgramRegisters *pr);
void input(ProgramRegisters *pr);
void output(ProgramRegisters *pr);
void jump(ProgramRegisters *pr);
void skipcond(ProgramRegisters *pr);

//add instruction
void add(ProgramRegisters *pr) {
    int temp = pr->ac_reg + twosComplement(pr->mbr);

    if((temp >= 0 && ((temp % 32767 != temp) && (temp % 32767 != 0))) || (temp < 0 && (temp % 32768 != temp) && (temp % 32768 != 0))) {
        pr->status_flag = 1;
        printf("OVERFLOW DETECTED\n");
    }

    pr->ac_reg = twosComplement(toDecFromComplement(pr->ac_reg) + pr->mbr);

    printf("ADD ---> AC = %d\n-----\n", pr->ac_reg);
}

//sub instruction
void sub(ProgramRegisters *pr) {
    int temp = pr->ac_reg - twosComplement(pr->mbr);

    if((temp >= 0 && ((temp % 32767 != temp) && (temp % 32767 != 0))) || (temp < 0 && (temp % 32768 != temp) && (temp % 32768 != 0))) {
        pr->status_flag = 1;
        printf("OVERFLOW DETECTED\n");
    }
    //printf("MBR %d - MBR TWOS - %d - AC %d\n", mbr, twosComplement(mbr), ac_reg);

    pr->ac_reg = twosComplement(toDecFromComplement(pr->ac_reg - twosComplement(pr->mbr)));

    printf("SUB ---> AC = %d\n-----\n", pr->ac_reg);
}

//load instruction
void load(ProgramRegisters *pr) {
    pr->ac_reg = twosComplement(pr->mem[pr->mar]);
    printf("LOAD ---> Loaded %d from AC from memory location: %d\n-----\n", pr->ac_reg, pr->mar);
}

//store instruction
void store(ProgramRegisters *pr) {
    pr->mem[pr->mar] = toDecFromComplement(pr->ac_reg);  //convert to unsigned representation from twos compl
    printf("\nSTORING %d IN %d, MEMORY NOW %d\n-----\n", pr->ac_reg, pr->mar, pr->mem[pr->mar]);
}

//input instruction
void input(ProgramRegisters *pr) {
    int x;

	printf("INPUT: Enter a number to be placed in the Accumulator: ");
	while((scanf("%d", &x) != 1) || !((x > -32768) && (x < 32767))) {
        printf("INVALID INPUT\n");
        printf("Enter a number to be placed in the Accumulator: ");

        int chr;
        do {
            chr = getchar();
        }
        while((chr != EOF) && (chr != '\n'));   //clear input buffer
	}
        pr->in_reg = x;
        printf("INPUT ---> AC = %d\n-----\n", pr->in_reg);
        pr->ac_reg = x;
    }

//output instruction
void output(ProgramRegisters *pr) {
	printf("OUTPUT ---> AC = %d\n-----\n", pr->out_reg);
}

//skipcond instruction
void skipcond(ProgramRegisters *pr) {
    if(pr->status_flag) {
        pr->pc = pr->pc + 1;
        pr->status_flag = 0;
    }
    else {
        return;
    }
}

//jump instruction
void jump(ProgramRegisters *pr) {
    pr->pc = pr->mar;
    printf("JUMPING TO %d IN MEMORY\n-----\n", pr->pc);
}

//convert binary represented as a string to decimal
int toDec(char *instr) {
    int i, decnum = 0, multiplier = 1;

    for(i = INSTR_LENGTH-1; i >= 0; i--) {
        if(instr[i] == '1') {
            decnum += multiplier;
        }
        if(isalpha(instr[i])) {
            printf("Bad input\n");
            return -1;
        }
        multiplier *= 2;
    }

    return decnum;
}

//convert binary to decimal
int toDecimalFromBinary(long long n) {
    int dec = 0, i = 0, multiplier = 1, remainder;

    while(n != 0) {
        remainder = n % 10;
        n /= 10;
        dec += remainder * multiplier;
        multiplier *= 2;
        i++;
    }
    return dec;
}

//convert decimal to twos complement representation
int twosComplement(int dec) {
    int binarynum[INSTR_LENGTH] = {0}, i = INSTR_LENGTH-1, j, number = 0, multiplier = 1;

    while(dec > 0) {
        binarynum[i] = dec % 2;
        dec /= 2;
        i--;
    }

    for(j = 15; j >= 0; j--) {
        if(j == 0 && binarynum[j] == 1) {
            number -= multiplier;
            return number;
        }
        if(binarynum[j] == 1 && j != 0) {
            number += multiplier;
        }
        multiplier *= 2;
    }
    return number;
}

//convert decimal in two's complement representation to non twos complement decimal
int toDecFromComplement(int compl) {
    if(compl < 0) {
        return (compl + 32768 + 32768);
    }
    else {
        return compl;
    }
}

//convert to binary long long number, opcode_true specifies
//if we want opcode or operand to be converted
long long toBinary(int dec, int opcode_true) {
    int binarynum[INSTR_LENGTH] = {0}, i = INSTR_LENGTH-1, j;
    long long ret = 0;

    while(dec > 0) {
        binarynum[i] = dec % 2;
        dec /= 2;
        i--;
    }

    if(opcode_true) {
        for(j = 0; j < 4; j++) {
            ret = ret * 10 + binarynum[j];
        }
        return ret;
    }
    else {
        for(j = 4; j < INSTR_LENGTH; j++) {
            ret = ret * 10 + binarynum[j];
        }
        return ret;
    }
}

//load some default memory contents
void loadDefaultMemory(ProgramRegisters *pr) {
    int default_mem[] = {12297, 4104, 8201, 32773, 32772, 32774, 20480, 32778, 16394, 12298, 4106, 0};
    int i, mem_size = sizeof(default_mem) / sizeof(int);

    for(i = 0; i < mem_size; i++) {
        pr->mem[i] = default_mem[i];
    }

    printf("\n=========================================================================\n");
    printf("\t\tLOADED DEFAULT MEMORY CONTENTS");
    printf("\n=========================================================================\n\n");

}

//take input from command line
int manualInput(ProgramRegisters *pr) {
    char input[INSTR_LENGTH+1] = "";

    printf("Enter an instruction to be stored in memory or exit:\n");
    gets(input);

    if(!strcmp(input, "stop") || !strcmp(input, "exit")) {
        printf("\n-----\nStopping manual input mode\n-----\n");
        return 0;
    }
    else {
        int i, input_dec = toDec(input);

        if(input_dec == -1) {
            printf("Invalid input. Please enter non alphabetical instruction\n");
            return 1;
        }
        for(i = 0; i < SIZE; i++) {
            if(pr->mem[i] == 0) {
                pr->mem[i] = input_dec;
                printf("Stored %s in memory location %d (0x", input, i);
                printOperand(i);
                printf(")\n-----\n");
                break;
            }
        }
        return 1;
    }
}

//read memory from file and print load contents
int fileInput(char *filename, ProgramRegisters *pr) {
    FILE *fp = fopen(filename, "r");
    char buff[SIZE][INSTR_LENGTH+1];
    int buff_index = 0, i;

    if(fp == NULL) {
        printf("Provided file '%s' does not exist\n", filename);
        return -1;
    }

    printf("\n=========================================================================\n");
    printf("\t\tREADING MEMORY FROM THE FILE");
    printf("\n=========================================================================\n\n");

    while(!feof(fp)) {
        char temp[INSTR_LENGTH+1];
        fscanf(fp, "%s", temp);
        strcpy(buff[buff_index], temp);
        printf("%d (0x", buff_index);
        printOperand(buff_index);
        printf(") --- %s\n", buff[buff_index]);
        pr->mem[buff_index] = toDec(buff[buff_index]);
        buff_index++;
    }

    printf("\n============================================================================\n");
    printf("\tASSEMBLY INSTRUCTIONS STORED IN MEMROY WITH 12 BIT OPERAND FOLLOWED \n\tOPERAND IS DISPLAYED ONLY IF IT IS BEING USED BY THE INSTRUCTION");
    printf("\n============================================================================\n");

    fclose(fp);
}

//print assembly instructions with following operands if they are needed
//this assumes that after halt there are no more instructions in memory
void printAssemblyInstructions(ProgramRegisters pr) {
    int i;

    for(i = 0; i < SIZE; i++) {
        if(pr.mem[i] != 0) {
            long long val = toBinary(pr.mem[i], 1);
            printf("%s ", getOperation(val));
            if(!strcmp(getOperation(val), "INPUT") || !strcmp(getOperation(val), "OUTPUT") || !strcmp(getOperation(val), "HALT") || !strcmp(getOperation(val), "SKIPCOND")) {
                printf("\n");
                continue;
            }
            printOperand(toDecimalFromBinary(toBinary(pr.mem[i], 0)));
            printf("\n");
        }
        else {
            printf("%s\n", getOperation(toBinary(pr.mem[i], 1)));
            break;
        }
    }
    printf("\n=========================================================================\n");
}

//print memory contents that are not 0s
void printNonEmptyMemory(ProgramRegisters pr) {
    int i;

    printf("\n=========================================================================\n");
    printf("\t\tMEMORY CONTENTS AFTER EXECUTION");
    printf("\n=========================================================================\n");

    for(i = 0; i < SIZE; i++) {
        if(pr.mem[i] != 0) {
            printf("%d (0x", i);
            printOperand(i);
            printf(") --- ");
            int binarynum[INSTR_LENGTH] = {0}, k = INSTR_LENGTH-1, j, dec = pr.mem[i];
            while(dec > 0) {
                binarynum[k] = dec % 2;
                dec /= 2;
                k--;
            }
            for (j = 0; j < INSTR_LENGTH; j++) {
                printf("%d", binarynum[j]);
            }
            printf("\n");
        }
    }
    printf("\nAC = %d\n", pr.ac_reg);
    printf("INPUT REG = %d\nOUTPUT REG = %d\n", pr.in_reg, pr.out_reg);
}

//print operand bits
void printOperand(int dec) {
    int binarynum[12] = {0}, i = 11, j;

    while(dec > 0) {
        binarynum[i] = dec % 2;
        dec /= 2;
        i--;
    }

    for (j = 0; j < 12; j++) {
        printf("%d", binarynum[j]);
    }
}

//print operation name
char *getOperation(int code) {
    switch(code) {
        case 0:
            return "HALT";
        case 1:
            return "ADD";
        case 10:
            return "SUB";
        case 11:
            return "LOAD";
        case 100:
            return "STORE";
        case 101:
            return "INPUT";
        case 110:
            return "OUTPUT";
        case 111:
            return "SKIPCOND";
        case 1000:
            return "JUMP";
        case 1001:
            return "AND";
        case 1010:
            return "OR";
    }
}

#endif // ACE3_H_INCLUDED
