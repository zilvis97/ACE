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

#include "ace3.h"

int main(int argc, char *argv[])
{
    int halt = 0, i;
    ProgramRegisters pr;
    pr.pc = 0;
    pr.in_reg = 0;
    pr.out_reg = 0;
    pr.status_flag = 0;

    for(i = 0; i < SIZE; i++) {
        pr.mem[i] = 0;
    }

    if(argc < 2) {
        printf("Not enough arguments provided.\n-d for default memory input\n-f for memory input from file\n-c for entering memory from command line\n");
        return 0;
    }
    if(argc >= 2) {
        if(!strcmp(argv[1], "-d")) {
            loadDefaultMemory(&pr);
        }
        else if(!strcmp(argv[1], "-c")) {
            while(manualInput(&pr)) {}
                printf("\n============================================================================\n");
                printf("\tASSEMBLY INSTRUCTIONS STORED IN MEMROY WITH 12 BIT OPERAND FOLLOWED \n\tOPERAND IS DISPLAYED ONLY IF IT IS BEING USED BY THE INSTRUCTION");
                printf("\n============================================================================\n");
        }
        else if(argc == 3 && !strcmp(argv[1], "-f")) {
            char *file_name = argv[2];
            if(fileInput(file_name, &pr) == -1) {
                return 0;
            }
        }
        else {
            printf("Wrong argument provided.\n-d for default memory input\n-f for memory input from file\n-c for entering memory from command line\n");
        }
    }

    printAssemblyInstructions(pr);

    //run the program
    while(!halt) {
        pr.mar = pr.pc;
        pr.instr_reg = pr.mem[pr.mar];
        pr.pc++;
        int opcode = toBinary(pr.instr_reg, 1);
        pr.mar = toDecimalFromBinary(toBinary(pr.instr_reg, 0));  //operand
        switch(opcode) {
            case 0:
                printf("END OF PROGRAM - HALT\n");
                halt = 1;
                break;
            case 1:
                pr.mbr = pr.mem[pr.mar];
                add(&pr);
                break;
            case 10:
                pr.mbr = pr.mem[pr.mar];
                sub(&pr);
                break;
            case 11:
                pr.mbr = pr.mem[pr.mar];
                load(&pr);
                break;
            case 100:
                pr.mbr = pr.mem[pr.mar];
                store(&pr);
                break;
            case 101:
                input(&pr);
                break;
            case 110:
                pr.out_reg = twosComplement(pr.ac_reg);
                output(&pr);
                break;
            case 111:
                skipcond(&pr);
                break;
            case 1000:
                jump(&pr);
                break;
        }
    }

    printNonEmptyMemory(pr);

    return 0;
}
