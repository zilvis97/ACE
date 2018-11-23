void skip(StatusFlag *fptr, ProgramRegisters *ptr){
    //Resets the status flag
    fptr->statusFlag = 0;
    //Sets the memory address register to the instruction pointed to by the PC
    ptr->memoryAddressReg = ptr->programCounter; 
    ptr->programCounter += 1; //sets the pc to the next instruction address
    ptr->instructionReg = cs210Memory[ptr->programCounter]; //sets instructionReg to next instruction.
}