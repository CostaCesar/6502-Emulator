#include "CPU.h"
using namespace Instruction;

uint32_t CPU::Execute(uint32_t cycles_total, Memory& memory)
{
    uint32_t cycles_ran = 0;
    
    for(cycles_ran; cycles_ran < cycles_total;)
    {
        Opcode instruction = FetchInstruction(cycles_ran, memory);
        Byte byte_Value = 0;
        Word word_Value = 0;
        switch (instruction)
        {
        case LDA_IM:
            RegA = FetchByte(cycles_ran, memory);
            SetStatus_NegvZero(RegA);
            break;
        case LDA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Load_Register(cycles_ran, RegA, byte_Value, memory);
            break;
        case LDA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Load_Register(cycles_ran, RegA, byte_Value, memory);
            break;
        case LDA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case LDA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Load_Register(cycles_ran, RegA, word_Value, memory);
            break;
        case LDX_IM:
            RegX = FetchByte(cycles_ran, memory);
            SetStatus_NegvZero(RegX);
            break;
        case LDX_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Load_Register(cycles_ran, RegX, byte_Value, memory);
            break;
        case LDX_ZPY:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegY);
            Load_Register(cycles_ran, RegX, byte_Value, memory);
            break;
        case LDX_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Load_Register(cycles_ran, RegX, word_Value, memory);
            break;
        case LDX_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Load_Register(cycles_ran, RegX, word_Value, memory);
            break;
        case LDY_IM:
            RegY = FetchByte(cycles_ran, memory);
            SetStatus_NegvZero(RegY);
            break;
        case LDY_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Load_Register(cycles_ran, RegY, byte_Value, memory);
            break;
        case LDY_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Load_Register(cycles_ran, RegY, byte_Value, memory);
            break;
        case LDY_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Load_Register(cycles_ran, RegY, word_Value, memory);
            break;
        case LDY_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Load_Register(cycles_ran, RegY, word_Value, memory);
            break;
        case STA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegA, cycles_ran);
            break;
        case STA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            memory.WriteByte(byte_Value, RegA, cycles_ran);
            break;
        case STA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case STA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case STA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegY);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            break;
        case STA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value + RegX, memory);
            memory.WriteByte(word_Value, RegA, cycles_ran);
            // Cycle used to secure memory in case of overflow in the addition of the address
            cycles_ran++;   
            break;
        case STA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            memory.WriteByte(word_Value + RegY, RegA, cycles_ran);
            // Cycle used to secure memory in case of overflow in the addition of the address   
            cycles_ran++;
            break;
        case STX_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegX, cycles_ran);
            break;  
        case STX_ZPY:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegY);
            memory.WriteByte(byte_Value, RegX, cycles_ran);
            break; 
        case STX_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegX, cycles_ran);
            break; 
        case STY_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            memory.WriteByte(byte_Value, RegY, cycles_ran);
            break;  
        case STY_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            memory.WriteByte(byte_Value, RegY, cycles_ran);
            break; 
        case STY_AB:
            word_Value = FetchWord(cycles_ran, memory);
            memory.WriteByte(word_Value, RegY, cycles_ran);
            break; 
        case JSR:
            word_Value = FetchWord(cycles_ran, memory);
            Push_ProgCount_Stack(cycles_ran, memory);
            ProgramCounter = word_Value;
            cycles_ran++;
            break;
        case RTS:
            ProgramCounter = Pop_PC_Stack(cycles_ran, memory);
            break;
        case JMP_AB:
            word_Value = FetchWord(cycles_ran, memory);
            ProgramCounter = word_Value;
            break;
        case JMP_ID:
            word_Value = FetchWord(cycles_ran, memory);
            if(ChipModel == CHIP_STANDART && (word_Value + 1) % 0x0100 == 0)
            {
                byte_Value = ReadByte(cycles_ran, ((int) word_Value - 0x00FF), memory);
                word_Value = (byte_Value << 8) + ReadByte(cycles_ran, word_Value, memory);
            }
            else word_Value = ReadWord(cycles_ran, word_Value, memory);
            ProgramCounter = word_Value;
            break;
        case TSX:
            RegX = StackPointer;
            cycles_ran++;
            SetStatus_NegvZero(RegX);
            break;
        case TXS:
            StackPointer = RegX;
            cycles_ran++;
            break;    
        case PHA:
            PushByte_Stack(cycles_ran, RegA, memory);
            break;     
        case PHP:
            PushByte_Stack(cycles_ran, FlagStatus, memory);
            break;    
        case PLA:
            RegA = PopByte_Stack(cycles_ran, memory);
            SetStatus_NegvZero(RegA);
            cycles_ran++;
            break; 
        case PLP:
            FlagStatus = PopByte_Stack(cycles_ran, memory);
            cycles_ran++;
            break;
        case AND_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            RegA &= byte_Value;
            SetStatus_NegvZero(RegA);
            break;
        case AND_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Logical_Operate(cycles_ran, byte_Value, '&', memory);
            break; 
        case AND_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Logical_Operate(cycles_ran, byte_Value, '&', memory);
            break;
        case AND_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
        case AND_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
        case AND_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break; 
        case AND_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
        case AND_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '&', memory);
            break;
         case ORA_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            RegA |= byte_Value;
            SetStatus_NegvZero(RegA);
            break;
        case ORA_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Logical_Operate(cycles_ran, byte_Value, '|', memory);
            break; 
        case ORA_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Logical_Operate(cycles_ran, byte_Value, '|', memory);
            break;
        case ORA_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
        case ORA_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
        case ORA_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break; 
        case ORA_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
        case ORA_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '|', memory);
            break;
         case EOR_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            RegA ^= byte_Value;
            SetStatus_NegvZero(RegA);
            break;
        case EOR_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            Logical_Operate(cycles_ran, byte_Value, '^', memory);
            break; 
        case EOR_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Logical_Operate(cycles_ran, byte_Value, '^', memory);
            break;
        case EOR_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case EOR_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case EOR_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break; 
        case EOR_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case EOR_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            Logical_Operate(cycles_ran, word_Value, '^', memory);
            break;
        case BIT_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, (Word) byte_Value, memory);
            Flags.Zero = !(RegA & byte_Value);
            FlagStatus |= (byte_Value & 0b11000000);
            break;
        case BIT_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Flags.Zero = !(RegA & byte_Value);
            FlagStatus |= (byte_Value & 0b11000000);
            break;
        case DEC_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            Memory_Increment(cycles_ran, word_Value, -1, memory);
            break;
        case DEC_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Memory_Increment(cycles_ran, (Word) byte_Value, -1, memory);
            break;
        case DEC_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Memory_Increment(cycles_ran, word_Value, -1, memory);
            break;
        case DEC_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            Memory_Increment(cycles_ran, word_Value, -1, memory);
            break;
        case INC_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            Memory_Increment(cycles_ran, word_Value, 1, memory);
            break;
        case INC_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            Memory_Increment(cycles_ran, (Word) byte_Value, 1, memory);
            break;
        case INC_AB:
            word_Value = FetchWord(cycles_ran, memory);
            Memory_Increment(cycles_ran, word_Value, 1, memory);
            break;
        case INC_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            Memory_Increment(cycles_ran, word_Value, 1, memory);
            break;
        case ASL_RGA:
            Shift_Value_WithZero(cycles_ran, RegA, '<');
            break;
        case ASL_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case ASL_ZPX:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Byte) word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte((Byte) word_Value, byte_Value, cycles_ran);
            break;
        case ASL_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case ASL_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case LSR_RGA:
            Shift_Value_WithZero(cycles_ran, RegA, '>');
            break;
        case LSR_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case LSR_ZPX:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Byte) word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte((Byte) word_Value, byte_Value, cycles_ran);
            break;
        case LSR_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case LSR_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_WithZero(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case ROL_RGA:
            Shift_Value_Carrying(cycles_ran, RegA, '<');
            break;
        case ROL_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case ROL_ZPX:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Byte) word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '<');
            memory.WriteByte((Byte) word_Value, byte_Value, cycles_ran);
            break;
        case ROL_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case ROL_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '<');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case ROR_RGA:
            Shift_Value_Carrying(cycles_ran, RegA, '>');
            break;
        case ROR_ZP:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case ROR_ZPX:
            word_Value = FetchByte_AsWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Byte) word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '>');
            memory.WriteByte((Byte) word_Value, byte_Value, cycles_ran);
            break;
        case ROR_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case ROR_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            IncrementByRegister(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Shift_Value_Carrying(cycles_ran, byte_Value, '>');
            memory.WriteByte(word_Value, byte_Value, cycles_ran);
            break;
        case BRK:
            // ProgramCounter++;
            Push_ProgCount_Stack(cycles_ran, memory);
            PushByte_Stack(cycles_ran, FlagStatus, memory);
            ProgramCounter = ReadWord(cycles_ran, 0xFFFE, memory);
            Flags.Break = 1;
            break;
        case RTI:
            FlagStatus = PopByte_Stack(cycles_ran, memory);
            ProgramCounter = PopWord_Stack(cycles_ran, memory) + 1;
            break;
        case NOP:
            cycles_ran += 2;
            break;
        case ADC_IM:
            byte_Value = FetchByte(cycles_ran, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case ADC_ZP:
            byte_Value = FetchByte(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, (Word) byte_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case ADC_ZPX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            byte_Value = ReadByte(cycles_ran, (Word) byte_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case ADC_AB:
            word_Value = FetchWord(cycles_ran, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case ADC_ABX:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegX);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case ADC_ABY:
            word_Value = FetchWord(cycles_ran, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case ADC_IDX:
            byte_Value = FetchByte(cycles_ran, memory);
            IncrementByRegister(cycles_ran, byte_Value, RegX);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case ADC_IDY:
            byte_Value = FetchByte(cycles_ran, memory);
            word_Value = ReadWord(cycles_ran, byte_Value, memory);
            Check_PageCross(cycles_ran, word_Value, RegY);
            byte_Value = ReadByte(cycles_ran, word_Value, memory);
            Math_Add(cycles_ran, byte_Value);
            break;
        case SBC_IM:
            break;
        case SBC_ZP:
            break;
        case SBC_ZPX:
            break;
        case SBC_AB:
            break;
        case SBC_ABX:
            break;
        case SBC_ABY:
            break;
        case SBC_IDX:
            break;
        case SBC_IDY:
            break;
        default:
            fprintf(stderr, "Unknow instruction \"%#x\" ", instruction);
            return cycles_ran;
        }
    }
    return cycles_ran;
}