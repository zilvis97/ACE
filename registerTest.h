#include <stdint.h>
int16_t arrayTest1[4096];

void initializeRegisters();
void fillArray();

struct programRegisters{
	unsigned int accumulaltor:16;
	unsigned int instructionReg:16;
	unsigned int memoryBufferReg:16;
	unsigned int programCounter:16;
	unsigned int memoryAddressReg:12;
};
