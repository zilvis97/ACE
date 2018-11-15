#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void load(int operand); //pass operand decimal
void store(int operand);

int mem[4096];
int ac_reg;

//int argc, string args[]
int main()
{
    FILE *fp = fopen("labas.txt", "r");

    char buff[4096][17];
    int buff_index = 0, i;

    while(!feof(fp)) {
        char temp[17];
        fscanf(fp, "%s", temp);
        strcpy(buff[buff_index], temp);
        buff_index++;
    }

    for(i = 0; i < buff_index; i++) {
        printf("%d --- %s\n", i, buff[i]);
    }
    printf("\n====================================================================\n");

    //convert binary string to decimal and store to memory
    for(i = 0; i < buff_index; i++) {
        int instruction = toDec(buff[i]);
        mem[i] = instruction;
        //printf("%d --- %d", i, mem[i]);
        long long val = toBinary(mem[i], 1);
        printf("%s", getOperation(val));
        printOperand(toDecimalFromBinary(toBinary(mem[i], 0)));
    }

    printf("\n====================================================================\n");

    for(i = 0; i < buff_index; i++) {
        char operation[20];
        strcpy(operation, getOperation(toBinary(mem[i], 1)));
//        switch(operation) {
//            case "HALT":
//                printf("END OF PROGRAM - HALT\n");
//                return 0;
//            case "ADD":
//                add();
//                break;
//            case "SUB":
//                sub();
//                break;
//            case "LOAD":
//                load(mem[i]);
//                break;
//            case "STORE":
//                store(mem[i]);
//                break;
//            case "INPUT":
//                input();
//                break;
//            case "OUTPUT":
//                output();
//                break;
//            case "JUMP"
//                jump();
//                break;
//            case "SKIPCOND":
//                skipcond();
//                break;
//        }
        if(strcmp(getOperation(toBinary(mem[i], 1)), "LOAD") == 0) {
            load(toDecimalFromBinary(toBinary(mem[i], 0)));
            printf("AC - %d\n", ac_reg);
        }

        if(strcmp(getOperation(toBinary(mem[i], 1)), "STORE") == 0) {
            store(toDecimalFromBinary(toBinary(mem[i], 0)));
        }
    }

    printf("\n====================================================================\n");
    printNonEmptyMemory();

    fclose(fp);
    return 0;
}

void load(int operand) {
    ac_reg = twosComplement(mem[operand]);
}

void store(int operand) {
    mem[operand] = toDecFromComplement(ac_reg);  //convert to non twos complement
    printf("\nSTORING %d in %d, MEMORY NOW %d", ac_reg, operand, mem[operand]);
}

//to decimal from a string
int toDec(char *instr) {
    int i, decnum = 0, multiplier = 1;

    for(i = 15; i >= 0; i--) {
//        if(i == 0 && instr[i] == '1') {
//            decnum -= multiplier;
//            return decnum;
//        }
        if(instr[i] == '1') {
            decnum += multiplier;
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
        return (compl + 65536);
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
    char bin[13] = {'0'};
    int binarynum[12] = {0}, i = 11, j;

    while(dec > 0) {
        binarynum[i] = dec % 2;
        dec /= 2;
        i--;
    }

    for(j = 0; j < 12; j++) {
        if(binarynum[j] == 1) {
            bin[j] = '1';
        }
        else {
            bin[j] = '0';
        }
    }

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
            return"STORE";
        case 101:
            return"INPUT";
        case 110:
            return"OUTPUT";
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
