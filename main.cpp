#include "src/CPU.h"
#include "src/Memory.h"

int main(int argc, char ** argv)
{
    Memory memory;
    CPU processor;
    processor.Reset(memory);
    
    // start - Hacked code
    memory[0xFFFC] = Instruction::JSR;
    memory[0xFFFD] = 0xAA; 
    memory[0xFFFE] = 0xAA;
    memory[0xAAAA] = Instruction::LDA_IM;
    memory[0xAAAB] = 0xA;
    // end - Hacked code

    processor.Execute(8, memory);
    return 0;
}