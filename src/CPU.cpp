#include "CPU.h"

void CPU::Reset(Memory& memory)
{
    Reset(0xFFFC, memory);
}
void CPU::Reset(Word address, Memory& memory)
{
    ProgramCounter = address;
    StackPointer = 0x00FF;
    F_Carry = F_Decimal = F_Zero = F_Interupt = F_Break = F_OverFlow = F_Negative = 0;
    RegA = RegY = RegX = 0;
    memory.Initialise();
}

/* Read 1 byte from memory */
Byte CPU::ReadByte(uint32_t& cycles, Word address, const Memory& memory)
{
    Byte data = memory[address];
    cycles++;
    return data;
}

/* Read 1 byte from memory AND increment program counter */
Byte CPU::FetchByte(uint32_t& cycles, const Memory& memory)
{
    Byte data = ReadByte(cycles, ProgramCounter, memory);
    ProgramCounter++;
    return data;
}
/* Read instruction from memory AND increment program counter */
Instruction CPU::FetchInstruction(uint32_t& cycles, const Memory& memory)
{
    Byte data = ReadByte(cycles, ProgramCounter, memory);
    ProgramCounter++;
    return (Instruction) data;
}

/* Read 1 word (2 bytes) from memory */
/* NOTE: Both my platform (x86) and 6502 are little-endian */
Word CPU::ReadWord(uint32_t& cycles, Word address, const Memory& memory)
{
    // My platform is little endian
    Word data = memory[address];
    data |= (memory[++address] << 8);
    cycles += 2;
    return data;
}

/* Read 1 word (2 bytes) from memory AND increment program counter */
/* NOTE: Both my platform (x86) and 6502 are little-endian */
Word CPU::FetchWord(uint32_t& cycles, const Memory& memory)
{
    // 6502 is little endian
    Word data = ReadWord(cycles, ProgramCounter, memory);
    ProgramCounter += 2;
    return data;
}

/* Set flags required by a LDA operation */
void CPU::LD_SetStatus(Byte& cpu_register)
{
    F_Zero = (cpu_register == 0);
    F_Negative = (cpu_register & 0b10000000) > 0;
}
void CPU::LD_SetRegister(uint32_t& cycles, Byte& cpu_register, Word address, const Memory& memory)
{
    cpu_register = ReadByte(cycles, address, memory);
    LD_SetStatus(cpu_register);
}

/* Increment data by value from register, using up 1 cycle for it */
void CPU::IncrementByRegister(uint32_t& cycles, Byte& value, Byte cpu_register)
{
    value += cpu_register;
    cycles++;
}
void CPU::IncrementByRegister(uint32_t& cycles, Word& value, Byte cpu_register)
{
    value += cpu_register;
    cycles++;
}

/* Handles the additional cycle when a load instruction crosses the page border */
void CPU::Check_PageCross(uint32_t& cycles, Word& address, Byte offset)
{
    if((int) (address / 0xFF) < (int) (address + offset) / 0xFF)
        cycles++;
    address += offset;
}

/* Write current Program Counter (-1) to the Stack*/
void CPU::Push_PC_ToStack(uint32_t& cycles, Memory& memory)
{
    memory.WriteWord(StackPointer_ToWord() - 1, ProgramCounter, cycles);
    cycles++;
    StackPointer -= 2;
}
/* Get previous Program Counter from the Stack*/
void CPU::Pop_PC_FromStack(uint32_t& cycles, Memory& memory)
{
    ProgramCounter = ReadWord(cycles, StackPointer_ToWord() + 1, memory);
    cycles += 3;
    StackPointer += 2;
}

/* Convert the Stack Pointer's current address to a Word value*/
Word CPU::StackPointer_ToWord() const
{ return 0x0100 | StackPointer; }

uint32_t CPU::Execute(uint32_t cycles_total, Memory& memory)
{
    uint32_t cycles_ran = 0;
    for(cycles_ran; cycles_ran < cycles_total;)
    {
        Instruction instruction = FetchInstruction(cycles_ran, memory);
        Byte byte_Value = 0;
        Word word_Value = 0;
        switch (instruction)
        {
        case INS_LDA_IM:
            RegA = FetchByte(cycles_ran, memory);
            LD_SetStatus(RegA);
            break;
        case INS_LDA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegA, byte_Value, memory);
            break;
        case INS_LDA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            LD_SetRegister(cycles_ran, RegA, byte_Value, memory);
            break;
        case INS_LDA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case INS_LDA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case INS_LDA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case INS_LDA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case INS_LDA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            LD_SetRegister(cycles_ran, RegA, word_Value, memory);
            break;
        case INS_LDX_IM:
            RegX = FetchByte(cycles_ran, memory);
            LD_SetStatus(RegX);
            break;
        case INS_LDX_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegX, byte_Value, memory);
            break;
        case INS_LDX_ZPY:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegY);
            LD_SetRegister(cycles_ran, RegX, byte_Value, memory);
            break;
        case INS_LDX_AB:
            word_Value = FetchWord(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegX, word_Value, memory);
            break;
        case INS_LDX_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            LD_SetRegister(cycles_ran, RegX, word_Value, memory);
            break;
        case INS_LDY_IM:
            RegY = FetchByte(cycles_ran, memory);
            LD_SetStatus(RegY);
            break;
        case INS_LDY_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegY, byte_Value, memory);
            break;
        case INS_LDY_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            LD_SetRegister(cycles_ran, RegY, byte_Value, memory);
            break;
        case INS_LDY_AB:
            word_Value = FetchWord(cycles_ran, memory);
            LD_SetRegister(cycles_ran, RegY, word_Value, memory);
            break;
        case INS_LDY_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            LD_SetRegister(cycles_ran, RegY, word_Value, memory);
            break;
        case INS_STA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegA, cycles_ran);
            break;
        case INS_STA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            memory.WriteByte(byte_Value, RegA, cycles_ran);
            break;
        case INS_STA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case INS_STA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case INS_STA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegY);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case INS_STA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value + RegX, memory);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            // Cycle used to secure memory in case of overflow in the addition of the adrres
            cycles_ran++;   
            break;
        case INS_STA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            memory.WriteByte(word_Value + RegY, RegA, cycles_ran);
            // Cycle used to secure memory in case of overflow in the addition of the adrres   
            cycles_ran++;
            break;
        case INS_STX_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegX, cycles_ran);
            break;  
        case INS_STX_ZPY:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegY);
            memory.WriteByte(byte_Value, RegX, cycles_ran);
            break; 
        case INS_STX_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegX, cycles_ran);
            break; 
        case INS_STY_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegY, cycles_ran);
            break;  
        case INS_STY_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            memory.WriteByte(byte_Value, RegY, cycles_ran);
            break; 
        case INS_STY_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegY, cycles_ran);
            break; 
        case INS_JSR:
            word_Value = FetchWord(cycles_ran, memory);
            Push_PC_ToStack(cycles_ran, memory);
            ProgramCounter = word_Value;
            break;
        case INS_RTS:
            Pop_PC_FromStack(cycles_ran, memory);
            break;
        default:
            printf("Unknow instruction \"%#x\" ", instruction);
            return cycles_ran;
        }
    }
    return cycles_ran;
}