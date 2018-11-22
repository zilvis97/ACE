#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* 0 HALT (break)1 ADD2 SUB3 LOAD4 STORE5 INPUT6 OUTPUT7 SKIP*/

/*
1. take input as a string
2. convert string to dec with toDec() and store in memory
3. converting to binary function toBinary() allows us to chose which part we want opcode or operand
4. from binary num gained in step 3 we can convert opcode/operand to decimal separately
5. when working with AC take memory value convert it to two's complement, when storing ac to memory convert it back to non twos complement
*/
//or store convert binary -> dec to store the mem

char *getOperation(int code);
int toDec(char *operand);
long long toBinary(int dec, int opcode_true);
int toDecimalFromBinary(long long n);
void printOperand(int dec);
void printNonEmptyMemory();
int twosComplement(int dec);
int toDecFromComplement(int compl);
void loadDefaultMemory();
int fileInput(char *filename);
int manualInput();

void add(int mbr);
void sub(int mbr);
void load(int mar); //pass operand decimal
void store(int mar);
void input(int *in_reg);
void output(int *out_reg);

int mem[4096];
int ac_reg;
int status_flag = 0;
int pc = 0, instr_reg, mbr, mar, in_reg, out_reg;

//int argc, string args[]
int main(int argc, char *argv[])
{
    //FILE *fp = fopen("test-program-1.txt", "r");

    //char buff[4096][17];
    int halt = 0, i;

    if(argc < 2) {
        printf("Not enough arguments provided.\n-d for default memory input\n-f for memory input from file\n-c for entering memory from command line\n");
        return 0;
    }
    if(argc >= 2) {
        if(!strcmp(argv[1], "-d")) {
            loadDefaultMemory();
        }
        else if(!strcmp(argv[1], "-c")) {
            while(manualInput()) {}
        }
        else if(argc == 3 && !strcmp(argv[1], "-f")) {
            char *file_name = argv[2];
            if(fileInput(file_name) == -1) {
                return 0;
            }
        }
        else {
            printf("Wrong argument provided.\n-d for default memory input\n-f for memory input from file\n-c for entering memory from command line\n");
        }
    }

//    while(!feof(fp)) {
//        char temp[17];
//        fscanf(fp, "%s", temp);
//        strcpy(buff[buff_index], temp);
//        printf("%d --- %s\n", buff_index, buff[buff_index]);
//        buff_index++;
//    }
//
//    printf("\n====================================================================\n");

    //convert binary string to decimal and store to memory
//    for(i = 0; i < buff_index; i++) {
//        int instruction = toDec(buff[i]);
//        mem[i] = instruction;
//        //printf("%d --- %d", i, mem[i]);
//        long long val = toBinary(mem[i], 1);
//        printf("%s", getOperation(val));
//        printOperand(toDecimalFromBinary(toBinary(mem[i], 0)));
//        printf("DECIMAL REP = %d\n", instruction);
//    }
//
//    printf("\n====================================================================\n");

    while(!halt) {
        mar = pc;
        instr_reg = mem[mar];
        pc++;
        int opcode = toBinary(instr_reg, 1);
        mar = toDecimalFromBinary(toBinary(instr_reg, 0));  //operand
        switch(opcode) {
            case 0:
                printf("END OF PROGRAM - HALT\n");
                halt = 1;
                break;
            case 1:
                mbr = mem[mar];
                add(mbr);
                break;
            case 10:
                mbr = mem[mar];
                sub(mbr);
                break;
            case 11:
                mbr = mem[mar];
                load(mar);
                break;
            case 100:
                mbr = mem[mar];
                store(mar);
                break;
            case 101:
                input(&in_reg);
                break;
            case 110:
                out_reg = twosComplement(ac_reg);
                output(&out_reg);
                break;
            case 111:
                //skipcond();
//                break;
            case 1000:
                pc = mar;
                printf("JUMPING TO %d IN MEMORY\n", pc);
                break;
        }
    }

    printf("\n====================================================================\n");
    printNonEmptyMemory();

//    fclose(fp);
    return 0;
}

void add(int mbr) {
    int temp = ac_reg + twosComplement(mbr);

    if((temp >= 0 && ((temp % 32767 != temp) && (temp % 32767 != 0))) || (temp < 0 && (temp % 32768 != temp) && (temp % 32768 != 0))) {
        status_flag = 1;
        printf("OVERFLOW DETECTED\n");
    }

    ac_reg = twosComplement(toDecFromComplement(ac_reg) + mbr);

    printf("ADD --- AC = %d\n", ac_reg);
}

void sub(int mbr) {
    int temp = ac_reg - twosComplement(mbr);

    if((temp >= 0 && ((temp % 32767 != temp) && (temp % 32767 != 0))) || (temp < 0 && (temp % 32768 != temp) && (temp % 32768 != 0))) {
        status_flag = 1;
        printf("OVERFLOW DETECTED\n");
    }
    //printf("MBR %d - MBR TWOS - %d - AC %d\n", mbr, twosComplement(mbr), ac_reg);

    ac_reg = twosComplement(toDecFromComplement(ac_reg - twosComplement(mbr)));

    printf("SUB --- AC = %d\n", ac_reg);
}

void load(int mar) {
    ac_reg = twosComplement(mem[mar]);
    printf("Loaded %d from AC from memory location: %d\n", ac_reg, mar);
}

void store(int mar) {
    mem[mar] = toDecFromComplement(ac_reg);  //convert to unsigned representation from twos compl
    printf("\nSTORING %d in %d, MEMORY NOW %d\n", ac_reg, mar, mem[mar]);
}

void input(int *in_reg) {
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
        *in_reg = x;
        printf("IN_REG = %d\n", in_reg);
        ac_reg = x;
    }

void output(int *out_reg) {
	printf("AC = %d\n", out_reg);
}

//to decimal from a string
int toDec(char *instr) {
    int i, decnum = 0, multiplier = 1;

    for(i = 15; i >= 0; i--) {
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

//to decimal from a long long number
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

int twosComplement(int dec) {
    int binarynum[16] = {0}, i = 15, j, number = 0, multiplier = 1;

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
    int binarynum[16] = {0}, i = 15, j;
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
        for(j = 4; j < 16; j++) {
            ret = ret * 10 + binarynum[j];
        }
        return ret;
    }
}

void loadDefaultMemory() {
    int default_mem[] = {12297, 4104, 8201, 32773, 32772, 32774, 20480, 32778, 16394, 12298, 4106, 0};
    int i, mem_size = sizeof(default_mem) / sizeof(int);

    for(i = 0; i < mem_size; i++) {
        mem[i] = default_mem[i];
    }
}

int manualInput() {
    char input[17] = "";

    printf("Enter an instruction to be stored in memory:\n");
    gets(input);

    if(!strcmp(input, "stop") || !strcmp(input, "exit")) {
        printf("\nStopping manual input mode\n");
        return 0;
    }
    else {
        int i, input_dec = toDec(input);

        if(input_dec == -1) {
            printf("Invalid input. Please enter non alphabetical instruction\n");
            return 1;
        }
        for(i = 0; i < 4096; i++) {
            if(mem[i] == 0) {
                mem[i] = input_dec;
                printf("Stored %d in memory location %d\n", input_dec, i);
                break;
            }
        }
        return 1;
    }
}

int fileInput(char *filename) {
    FILE *fp = fopen(filename, "r");
    char buff[4096][17];
    int buff_index = 0, i;

    if(fp == NULL) {
        printf("Provided file '%s' does not exist\n", filename);
        return -1;
    }

    while(!feof(fp)) {
        char temp[17];
        fscanf(fp, "%s", temp);
        strcpy(buff[buff_index], temp);
        printf("%d --- %s\n", buff_index, buff[buff_index]);
        buff_index++;
    }
    printf("\n====================================================================\n");

    for(i = 0; i < buff_index; i++) {
        int instruction = toDec(buff[i]);
        mem[i] = instruction;
        long long val = toBinary(mem[i], 1);
        printf("%s", getOperation(val));
        printOperand(toDecimalFromBinary(toBinary(mem[i], 0)));
        printf("DECIMAL REP = %d\n", instruction);
    }
    printf("\n====================================================================\n");

    fclose(fp);
}

void printNonEmptyMemory() {
    int i;

    for(i = 0; i < 4096; i++) {
        if(mem[i] != 0) {
            printf("%d. %d --- TWOS COMPLEMENT %d --- NORMAL DEC %d\n", i, mem[i], twosComplement(mem[i]), toDecFromComplement(twosComplement(mem[i])));
        }
    }
}

//convertinti is decimal i binary ir tuo paciu addinti i stringa
void printOperand(int dec) {
    //char bin[13] = {'0'};
    int binarynum[12] = {0}, i = 11, j;

    while(dec > 0) {
        binarynum[i] = dec % 2;
        dec /= 2;
        i--;
    }

//    for(j = 0; j < 12; j++) {
//        if(binarynum[j] == 1) {
//            bin[j] = '1';
//        }
//        else {
//            bin[j] = '0';
//        }
//    }

    printf(" ");
    for (j = 0; j < 12; j++) {
        printf("%d", binarynum[j]);
    }
    //printf("-----%s", bin);
    printf("\n");
}

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
