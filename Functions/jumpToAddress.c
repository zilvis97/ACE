void jumpToAddr(StatusFlag *fptr, ProgramRegisters *ptr, int addressToGo) {
    fptr->statusFlag = 0;
    int memoryoperand = addressToGo<<4;
    memoryoperand = memoryoperand>>4;
    ptr->programCounter = ptr->memoryAddressReg;
    ptr->instructionReg = cs210Memory[ptr->programCounter];
}
