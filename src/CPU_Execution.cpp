#include "CPU.h"

uint32_t CPU::Execute(uint32_t cycles_total, Memory& memory)
{
    const Byte vec[] = {0, 1, 2};
    uint32_t cycles_ran;

    for(cycles_ran = 0; cycles_ran < cycles_total;)
    {
        Opcode instruction = FetchInstruction(cycles_ran, memory);
        Byte byte_Value = 0;
        Word word_Value = 0;
        switch (instruction)
        {
        case Set_6502::LDA_IM:
            RegA = FetchByte(cycles_ran, memory);
            SetStatus_NegvZero(RegA);
            break;
        case Set_6502::LDA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Load_Register(cycles_ran, RegA, byte_Value, memory);
            break;
        case Set_6502::LDA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Load_Register(cycles_ran, RegA, byte_Value, memory);
            break;
        case Set_6502::LDA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case Set_6502::LDA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case Set_6502::LDA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case Set_6502::LDA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case Set_6502::LDA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case Set_6502::LDX_IM:
            RegX = FetchByte(cycles_ran, memory);
            SetStatus_NegvZero(RegX);
            break;
        case Set_6502::LDX_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Load_Register(cycles_ran, RegX, byte_Value, memory);
            break;
        case Set_6502::LDX_ZPY:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegY);
            Load_Register(cycles_ran, RegX, byte_Value, memory);
            break;
        case Set_6502::LDX_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Load_Register(cycles_ran, RegX, word_Value, memory);
            break;
        case Set_6502::LDX_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Load_Register(cycles_ran, RegX, word_Value, memory);
            break;
        case Set_6502::LDY_IM:
            RegY = FetchByte(cycles_ran, memory);
            SetStatus_NegvZero(RegY);
            break;
        case Set_6502::LDY_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Load_Register(cycles_ran, RegY, byte_Value, memory);
            break;
        case Set_6502::LDY_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Load_Register(cycles_ran, RegY, byte_Value, memory);
            break;
        case Set_6502::LDY_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Load_Register(cycles_ran, RegY, word_Value, memory);
            break;
        case Set_6502::LDY_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Load_Register(cycles_ran, RegY, word_Value, memory);
            break;
        case Set_6502::STA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegA, cycles_ran);
            break;
        case Set_6502::STA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            memory.WriteByte(byte_Value, RegA, cycles_ran);
            break;
        case Set_6502::STA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case Set_6502::STA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case Set_6502::STA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegY);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case Set_6502::STA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value + RegX, memory);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            // Cycle used to secure memory if overflow occours in the adition of the address
            cycles_ran++;   
            break;
        case Set_6502::STA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            memory.WriteByte(word_Value + RegY, RegA, cycles_ran);
            // Cycle used to secure memory if overflow occours in the adition of the address   
            cycles_ran++;
            break;
        case Set_6502::STX_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegX, cycles_ran);
            break;  
        case Set_6502::STX_ZPY:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegY);
            memory.WriteByte(byte_Value, RegX, cycles_ran);
            break; 
        case Set_6502::STX_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegX, cycles_ran);
            break; 
        case Set_6502::STY_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegY, cycles_ran);
            break;  
        case Set_6502::STY_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            memory.WriteByte(byte_Value, RegY, cycles_ran);
            break; 
        case Set_6502::STY_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegY, cycles_ran);
            break; 
        case Set_6502::JSR:
            word_Value = FetchWord(cycles_ran, memory);
            Push_ProgCount_Stack(cycles_ran, memory);
            ProgramCounter = word_Value;
            cycles_ran++;
            break;
        case Set_6502::RTS:
            ProgramCounter = Pop_PC_Stack(cycles_ran, memory);
            break;
        case Set_6502::JMP_AB:
            word_Value = FetchWord(cycles_ran, memory);
            ProgramCounter = word_Value;
            break;
        case Set_6502::JMP_ID:
            word_Value = FetchWord(cycles_ran, memory);
            if(ChipModel == CHIP_STANDART && (word_Value + 1) % 0x0100 == 0)
            {
                byte_Value = ReadByte(cycles_ran, (Word) ((int) word_Value - 0x00FF), memory);
                word_Value = (byte_Value << 8) + ReadByte(cycles_ran, word_Value, memory);
            }
            else word_Value = ReadWord(cycles_ran, word_Value, memory);
            ProgramCounter = word_Value;
            break;
        case Set_6502::TSX:
            RegX = StackPointer;
            cycles_ran++;
            SetStatus_NegvZero(RegX);
            break;
        case Set_6502::TXS:
            StackPointer = RegX;
            cycles_ran++;
            break;    
        case Set_6502::PHA:
            PushByte_Stack(cycles_ran, RegA, memory);
            break;     
        case Set_6502::PHP:
            PushByte_Stack(cycles_ran, FlagStatus, memory);
            break;    
        case Set_6502::PLA:
            RegA = PopByte_Stack(cycles_ran, memory);
            SetStatus_NegvZero(RegA);
            cycles_ran++;
            break; 
        case Set_6502::PLP:
            FlagStatus = PopByte_Stack(cycles_ran, memory);
            cycles_ran++;
            break;
        case Set_6502::AND_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            RegA &= byte_Value;
            SetStatus_NegvZero(RegA);
            break;
        case Set_6502::AND_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Logical_Operate(cycles_ran, byte_Value, '&', memory);
            break; 
        case Set_6502::AND_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Logical_Operate(cycles_ran, byte_Value, '&', memory);
            break;
        case Set_6502::AND_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
        case Set_6502::AND_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
        case Set_6502::AND_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break; 
        case Set_6502::AND_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
        case Set_6502::AND_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
         case Set_6502::ORA_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            RegA |= byte_Value;
            SetStatus_NegvZero(RegA);
            break;
        case Set_6502::ORA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Logical_Operate(cycles_ran, byte_Value, '|', memory);
            break; 
        case Set_6502::ORA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Logical_Operate(cycles_ran, byte_Value, '|', memory);
            break;
        case Set_6502::ORA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
        case Set_6502::ORA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
        case Set_6502::ORA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break; 
        case Set_6502::ORA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
        case Set_6502::ORA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
         case Set_6502::EOR_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            RegA ^= byte_Value;
            SetStatus_NegvZero(RegA);
            break;
        case Set_6502::EOR_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Logical_Operate(cycles_ran, byte_Value, '^', memory);
            break; 
        case Set_6502::EOR_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Logical_Operate(cycles_ran, byte_Value, '^', memory);
            break;
        case Set_6502::EOR_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case Set_6502::EOR_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case Set_6502::EOR_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break; 
        case Set_6502::EOR_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case Set_6502::EOR_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case Set_6502::BIT_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, (Word) byte_Value, memory);
            Flags.Zero = !(RegA & byte_Value);
            FlagStatus |= (byte_Value & 0b11000000);
            break;
        case Set_6502::BIT_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Flags.Zero = !(RegA & byte_Value);
            FlagStatus |= (byte_Value & 0b11000000);
            break;
        case Set_6502::DEC_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            Memory_Increment(cycles_ran, word_Value, -1, memory);
            break;
        case Set_6502::DEC_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Memory_Increment(cycles_ran, (Word) byte_Value, -1, memory);
            break;
        case Set_6502::DEC_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Memory_Increment(cycles_ran, word_Value, -1, memory);
            break;
        case Set_6502::DEC_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            Memory_Increment(cycles_ran, word_Value, -1, memory);
            break;
        case Set_6502::INC_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            Memory_Increment(cycles_ran, word_Value, 1, memory);
            break;
        case Set_6502::INC_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Memory_Increment(cycles_ran, (Word) byte_Value, 1, memory);
            break;
        case Set_6502::INC_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Memory_Increment(cycles_ran, word_Value, 1, memory);
            break;
        case Set_6502::INC_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            Memory_Increment(cycles_ran, word_Value, 1, memory);
            break;
        case Set_6502::ASL_RGA:
            Shift_Value_WithZero(cycles_ran, RegA, '<');
            break;
        case Set_6502::ASL_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::ASL_ZPX:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Byte) word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte((Byte) word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::ASL_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::ASL_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::LSR_RGA:
            Shift_Value_WithZero(cycles_ran, RegA, '>');
            break;
        case Set_6502::LSR_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::LSR_ZPX:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Byte) word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte((Byte) word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::LSR_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::LSR_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::ROL_RGA:
            Shift_Value_Carrying(cycles_ran, RegA, '<');
            break;
        case Set_6502::ROL_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::ROL_ZPX:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Byte) word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '<');
            memory.WriteByte((Byte) word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::ROL_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::ROL_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::ROR_RGA:
            Shift_Value_Carrying(cycles_ran, RegA, '>');
            break;
        case Set_6502::ROR_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::ROR_ZPX:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Byte) word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '>');
            memory.WriteByte((Byte) word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::ROR_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::ROR_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case Set_6502::BRK:
            // ProgramCounter++;
            Push_ProgCount_Stack(cycles_ran, memory);
            PushByte_Stack(cycles_ran, FlagStatus, memory);
            ProgramCounter = ReadWord(cycles_ran, 0xFFFE, memory);
            Flags.Break = 1;
            break;
        case Set_6502::RTI:
            FlagStatus = PopByte_Stack(cycles_ran, memory);
            ProgramCounter = PopWord_Stack(cycles_ran, memory) + 1;
            break;
        case Set_6502::NOP:
            cycles_ran += 2;
            break;
        case Set_6502::ADC_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case Set_6502::ADC_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, (Word) byte_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case Set_6502::ADC_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Word) byte_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case Set_6502::ADC_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case Set_6502::ADC_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case Set_6502::ADC_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case Set_6502::ADC_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case Set_6502::ADC_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case Set_6502::SBC_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            Math_Sub(cycles_ran, byte_Value);
            break;
        case Set_6502::SBC_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, (Word) byte_Value, memory);
            Math_Sub(cycles_ran, byte_Value);
            break;
        case Set_6502::SBC_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Word) byte_Value, memory);
            Math_Sub(cycles_ran, byte_Value);
            break;
        case Set_6502::SBC_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Sub(cycles_ran, byte_Value);
            break;
        case Set_6502::SBC_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Sub(cycles_ran, byte_Value);
            break;
        case Set_6502::SBC_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Sub(cycles_ran, byte_Value);
            break;
        case Set_6502::SBC_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Sub(cycles_ran, byte_Value);
            break;
        case Set_6502::SBC_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Sub(cycles_ran, byte_Value);
            break;
        case Set_6502::CLC:
            Flags.Carry = 0, cycles_ran++;
            break;
        case Set_6502::SEC:
            Flags.Carry = 1, cycles_ran++;
            break;
        case Set_6502::CLD:
            Flags.Decimal = 0, cycles_ran++;
            break;
        case Set_6502::SED:
            Flags.Decimal = 1, cycles_ran++;
            break;
        case Set_6502::CLI:
            Flags.Interupt = 0, cycles_ran++;
            break;
        case Set_6502::SEI:
            Flags.Interupt = 1, cycles_ran++;
            break;
        case Set_6502::CLV:
            Flags.OverFlow = 0, cycles_ran++;
            break;
        case Set_6502::TAX:
            RegX = RegA;
            cycles_ran++;
            SetStatus_NegvZero(RegX);
            break;
        case Set_6502::TAY:
            RegY = RegA;
            cycles_ran++;
            SetStatus_NegvZero(RegY);
            break;
        case Set_6502::TXA:
            RegA = RegX;
            cycles_ran++;
            SetStatus_NegvZero(RegA);
            break;
        case Set_6502::TYA:
            RegA = RegY;
            cycles_ran++;
            SetStatus_NegvZero(RegA);
            break;
        case Set_6502::DEX:
            IncrementByRegister(cycles_ran, RegX, -1);
            SetStatus_NegvZero(RegX);
            break;
        case Set_6502::DEY:
            IncrementByRegister(cycles_ran, RegY, -1);
            SetStatus_NegvZero(RegY);
            break;
        case Set_6502::INX:
            IncrementByRegister(cycles_ran, RegX, 1);
            SetStatus_NegvZero(RegX);
            break;
        case Set_6502::INY:
            IncrementByRegister(cycles_ran, RegY, 1);;
            SetStatus_NegvZero(RegY);
            break;
        case Set_6502::CMP_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            Compare(cycles_ran, byte_Value, RegA);
            break; 
        case Set_6502::CMP_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, byte_Value, memory);
            Compare(cycles_ran, byte_Value, RegA);
            break; 
        case Set_6502::CMP_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            byte_Value = ReadByte(cycles_ran, byte_Value, memory);
            Compare(cycles_ran, byte_Value, RegA);
            break;
        case Set_6502::CMP_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Compare(cycles_ran, byte_Value, RegA);
            break; 
        case Set_6502::CMP_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Compare(cycles_ran, byte_Value, RegA);
            break; 
        case Set_6502::CMP_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Compare(cycles_ran, byte_Value, RegA);
            break; 
        case Set_6502::CMP_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Compare(cycles_ran, byte_Value, RegA);
            break;
        case Set_6502::CMP_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Compare(cycles_ran, byte_Value, RegA);
            break;
        case Set_6502::CPX_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            Compare(cycles_ran, byte_Value, RegX);
            break; 
        case Set_6502::CPX_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, byte_Value, memory);
            Compare(cycles_ran, byte_Value, RegX);
            break; 
        case Set_6502::CPX_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Compare(cycles_ran, byte_Value, RegX);
            break; 
        case Set_6502::CPY_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            Compare(cycles_ran, byte_Value, RegY);
            break; 
        case Set_6502::CPY_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, byte_Value, memory);
            Compare(cycles_ran, byte_Value, RegY);
            break; 
        case Set_6502::CPY_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Compare(cycles_ran, byte_Value, RegY);
            break; 
        case Set_6502::BPL:
            byte_Value = FetchByte(cycles_ran, memory);
            Branch(cycles_ran, byte_Value, Flags.Negative == 0);
            break;
        case Set_6502::BMI:
            byte_Value = FetchByte(cycles_ran, memory);
            Branch(cycles_ran, byte_Value, Flags.Negative == 1);
            break;
        case Set_6502::BVC:
            byte_Value = FetchByte(cycles_ran, memory);
            Branch(cycles_ran, byte_Value, Flags.OverFlow == 0);
            break;
        case Set_6502::BVS:
            byte_Value = FetchByte(cycles_ran, memory);
            Branch(cycles_ran, byte_Value, Flags.OverFlow == 1);
            break;
        case Set_6502::BCC:
            byte_Value = FetchByte(cycles_ran, memory);
            Branch(cycles_ran, byte_Value, Flags.Carry == 0);
            break;
        case Set_6502::BCS:
            byte_Value = FetchByte(cycles_ran, memory);
            Branch(cycles_ran, byte_Value, Flags.Carry == 1);
            break;
        case Set_6502::BNE:
            byte_Value = FetchByte(cycles_ran, memory);
            Branch(cycles_ran, byte_Value, Flags.Zero == 0);
            break;
        case Set_6502::BEQ:
            byte_Value = FetchByte(cycles_ran, memory);
            Branch(cycles_ran, byte_Value, Flags.Zero == 1);
            break;
        default:
            fprintf(stderr, "Unknow instruction \"%#x\" ", instruction);
            return cycles_ran;
        }
    }
    return cycles_ran;
}