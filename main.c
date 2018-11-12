#include <stdio.h>
#include <stdlib.h>

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
int toDec(char *operand);
void toBinary(int dec);

int main()
{
    FILE *fp = fopen("labas.txt", "r");
    FILE *write = fopen("rasom.txt", "w+");
    char buff[4096][17];
    int buff_index = 0;

    while(!feof(fp)) {
        char temp[17];
        fscanf(fp, "%s", temp);
        strcpy(buff[buff_index], temp);
        buff_index++;
    }

    int i;
    for(i = 0; i < buff_index; i++) {
        printf("%d --- %s\n", i, buff[i]);
    }

    disasm(buff, buff_index);

    fclose(write);
    fclose(fp);
    //printf("%d", fclose(fp));
    return 0;
}

void disasm(char buff[][17], int size)
{
    printf("Zdrw from didsasm\n");
    int i;
    for(i = 0; i < size; i++) {
        int opcode, operand_dec;
        char *ptr = *(buff+i)+4;    //pointing to start of operand (5th character)
        char temp[5] = "\0";
        char operand[13] = "\0";
        strncpy(temp, buff[i], 4);  //copy opcode to temp
        strncpy(operand, ptr, 12);  //copy operand to operand
        opcode = atoi(temp);
        printf("%s --- %s --- OPERAND: --- %s ", buff[i], printOperation(opcode), operand);
        operand_dec = toDec(operand);
        printf("\nDECIMAL - %d", operand_dec);

        toBinary(operand_dec);
    }
}

char *printOperation(int code)
{
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

int toDec(char *operand)
{
    int i, decnum = 0, multiplier = 1;

    for(i = 11; i >= 0; i--) {
        if(operand[i] == '1') {
            decnum += multiplier;
        }
        multiplier *= 2;
    }

    return decnum;
}

void toBinary(int dec)
{
    int binarynum[12] = {0}, i = 11;

    while(dec > 0) {
        binarynum[i] = dec % 2;
        dec /= 2;
        i--;
    }

    printf("\nBINARY - ");
    int j;
    for(j = 0; j < 12; j++) {
        printf("%d", binarynum[j]);
    }
    printf("\n");
}
