#ifndef REGISTERTEST_H
#define REGISTERTEST_H
#include <stdint.h>

int16_t cs210Memory[4096];

typedef struct{
	signed int accumulaltor:16;
	unsigned int instructionReg:16;
	unsigned int memoryBufferReg:16;
	unsigned int programCounter:16;
	unsigned int memoryAddressReg:12;
}ProgramRegisters;

typedef struct{
	unsigned int statusFlag:1;
}StatusFlag;

char *printOperation(int code);
int toDec(char *operand, int length);
long long int toBinary(int dec, int opcode_true);
int toDecimalFromBinary(long long n);
void printOperand(int dec);

#endif /* REGISTERTEST_H */