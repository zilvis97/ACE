#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
0 HALT (break)
1 ADD
2 SUB
3 LOAD
4 STORE
5 INPUT
6 OUTPUT
7 SKIP
*/

//or store convert binary -> dec to store the mem

void disasm(char buff[][17], int size);
char *printOperation(int code);
int toDec(char *operand, int length);
long long int toBinary(int dec, int opcode_true);

int mem[4096];

int main()
{
    FILE *fp = fopen("labas.txt", "r");
    //FILE *write = fopen("rasom.txt", "w+");
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

    //disasm(buff, buff_index);

    for(i = 0; i < buff_index; i++) {
        int instruction = toDec(buff[i], 16);
        mem[i] = instruction;
        printf("%d --- %d", i, mem[i]);
        long long int val = toBinary(mem[i], 0);
        printf("--- OPCODE --- %lli", toBinary(mem[i], 1));
        printf("--- OPERAND --- %lli", toBinary(mem[i], 0));
        printf(" DECIMAL FROM OPERAND --- %d\n", toDecimalFromBinary(val));
    }

    fclose(fp);
    return 0;
}

void disasm(char buff[][17], int size)
{
    int i;
    for(i = 0; i < size; i++) {
        int opcode, operand_dec, instruction;
        char *ptr = *(buff+i)+4;    //pointing to start of operand (5th character)
        char opcd[5] = "\0", operand[13] = "\0";

        strncpy(opcd, buff[i], 4);  //copy opcode to temp
        strncpy(operand, ptr, 12);  //copy operand to operand

        opcode = atoi(opcd);
        //operand_dec = toDec(operand);
        instruction = toDec(buff[i], 16);
        mem[i] = instruction;

        printf("%s --- %s --- OPERAND: --- %s DECIMAL - %d\n", buff[i], printOperation(opcode), operand, instruction);

        //toBinary(instruction);
    }
}

int toDec(char *instr, int length) {
    int i, decnum = 0, multiplier = 1;

    for(i = length-1; i >= 0; i--) {
        if(instr[i] == '1') {
            decnum += multiplier;
        }
        multiplier *= 2;
    }

    return decnum;
}

int toDecimalFromBinary(long long n) {
    int dec = 0, i = 0, remainder;

    while(n != 0) {
        remainder = n % 10;
        n /= 10;
        dec += remainder * pow(2, i);
        i++;
    }
    return dec;
}

long long int toBinary(int dec, int opcode_true)
{
    int binarynum[16] = {0}, i = 15, j;
    long long int ret = 0;

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

//    printf("\nBINARY - ");
//    for(j = 0; j < 16; j++) {
//        printf("%d", binarynum[j]);
//    }
//    printf("\n");
}

char *printOperation(int code) {
    switch(code) {
        case 0:
            return "HLT";
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
            return "JMP";
        case 1001:
            return "AND";
        case 1010:
            return "OR";
    }
}
