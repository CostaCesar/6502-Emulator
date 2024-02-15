#ifndef BASE_H
#define BASE_H

using Byte = unsigned char;
using Word = unsigned short;

/* Opcodes */
namespace Instruction
{
    enum Opcode: Byte
    {
        // >>>>> Memory <<<<<
        // > LDA
        LDA_IM  = 0xA9, // 2 cycles: Load to RegA imediate value
        LDA_ZP  = 0xA5, // 3 cycles: Load to RegA value from ZP-memory (0x0000 -> 0x00FF)
        LDA_ZPX = 0xB5, // 4 cycles: Load to RegA value from ZP-memory + offset from RegX
        LDA_AB  = 0xAD, // 4 cycles: Load to RegA value from memory
        LDA_ABX = 0xBD, // 4 cycles: (+1 if cross boundary): Load to RegA value from memory + offset from RegX
        LDA_ABY = 0xB9, // 4 cycles: (+1 if cross boundary): Load to RegA value from memory + offset from RegY
        LDA_IDX = 0xA1, // 6 cycles: Load to RegA value from position pointed by [ZP-memory + offset from RegX]
        LDA_IDY = 0xB1, // 5 cycles: (+1 if cross boundary): Load to RegA value from position ZP-memory + offset from RegY
        
        // > LDX
        LDX_IM  = 0xA0, // 2 cycles: Load to RegX imediate value
        LDX_ZP  = 0xA4, // 3 cycles: Load to RegX value from ZP-memory (0x0000 -> 0x00FF)
        LDX_ZPY = 0xB4, // 4 cycles: Load to RegX value from ZP-memory + offset from RegY
        LDX_AB  = 0xAC, // 4 cycles: Load to RegX value from memory
        LDX_ABY = 0xBC, // 4 cycles: (+1 if cross boundary): Load to RegX value from memory + offset from RegY

        // > LDY
        LDY_IM  = 0xA2, // 2 cycles: Load to RegY imediate value
        LDY_ZP  = 0xA6, // 3 cycles: Load to RegY value from ZP-memory (0x0000 -> 0x00FF)
        LDY_ZPX = 0xB6, // 4 cycles: Load to RegY value from ZP-memory + offset from RegX
        LDY_AB  = 0xAE, // 4 cycles: Load to RegY value from memory
        LDY_ABX = 0xBE, // 4 cycles: (+1 if cross boundary): Load to RegY value from memory + offset from RegX

        // > STA
        STA_ZP  = 0x85, // 3 cycles: Store RegA value to ZP-memory (0x0000 -> 0x00FF)
        STA_ZPX = 0x95, // 4 cycles: Store RegA value to ZP-memory + offset from RegX
        STA_AB  = 0x8D, // 4 cycles: Store RegA value to memory
        STA_ABX = 0x9D, // 5 cycles: Store RegA value to memory + offset from RegX
        STA_ABY = 0x99, // 5 cycles: Store RegA value tomemory + offset from RegY
        STA_IDX = 0x81, // 6 cycles: Store RegA value to position pointed by [ZP-memory + offset from RegX]
        STA_IDY = 0x91, // 6 cycles: Store RegA value to position ZP-memory + offset from RegY

        // > STX
        STX_ZP  = 0x86, // 3 cycles: Store RegX value to ZP-memory (0x0000 -> 0x00FF)
        STX_ZPY = 0x96, // 4 cycles: Store RegX value to ZP-memory + offset from RegY
        STX_AB  = 0x8E, // 4 cycles: Store RegX value to memory

        // > STY
        STY_ZP  = 0x84, // 3 cycles: Store RegY value to ZP-memory (0x0000 -> 0x00FF)
        STY_ZPX = 0x94, // 4 cycles: Store RegY value to ZP-memory + offset from RegX
        STY_AB  = 0x8C, // 4 cycles: Store RegY value to memory

        // > DEC
        DEC_ZP  = 0xC6, // 5 cycles: Decrement value in ZP-memory address (0x0000 -> 0x00FF)
        DEC_ZPX = 0xD6, // 6 cycles: Decrement value in ZP-memory address + offset from Regx
        DEC_AB  = 0xCE, // 6 cycles: Decrement value in memory
        DEC_ABX = 0xDE, // 7 cycles: Decrement value in memory + offset from RegX

        // > INC
        INC_ZP  = 0xE6, // 5 cycles: Increment value in ZP-memory address (0x0000 -> 0x00FF)
        INC_ZPX = 0xF6, // 6 cycles: Increment value in ZP-memory address + offset from Regx
        INC_AB  = 0xEE, // 6 cycles: Increment value in memory
        INC_ABX = 0xFE, // 7 cycles: Increment value in memory + offset from RegX
        
        // >>>>> Jump <<<<<
        JSR     = 0x20, // 6 cycles: Branch to subroutine
        RTS     = 0x60, // 6 cycles: Return from subroutine
        JMP_AB  = 0x4C, // 3 cycles: Jump to position in memory directly
        JMP_ID  = 0x6C, // 3 cycles: Jump to position in memory pointed by another position

        // >>>>> Math <<<<<
        // > ADC
        ADC_IM  = 0x69, // 2 Cycles: Add to RegA an imediate value
        ADC_ZP  = 0x65, // 3 Cycles: Add to RegA a ZP-memory (0x0000 -> 0x00FF) value
        ADC_ZPX = 0x75, // 4 Cycles: Add to RegA a ZP-memory + offset from RegX value
        ADC_AB  = 0x6D, // 4 Cycles: Add to RegA a value from memory
        ADC_ABX = 0x7D, // 4 Cycles (+1 if cross boundary): Add to RegA a value from memory + offset from RegX
        ADC_ABY = 0x79, // 4 Cycles (+1 if cross boundary): Add to RegA a value from memory + offset from RegY
        ADC_IDX = 0x61, // 6 Cycles: Add to RegA a value from position pointed by [ZP-memory + offset from RegX]
        ADC_IDY = 0x71, // 5 Cycles (+1 if cross boundary): Add to RegA a value from position ZP-memory + offset from RegY

        // > SBC
        SBC_IM  = 0xE9, // 2 Cycles: Subtract to RegA an imediate value
        SBC_ZP  = 0xE5, // 3 Cycles: Subtract to RegA a ZP-memory (0x0000 -> 0x00FF) value
        SBC_ZPX = 0xF5, // 4 Cycles: Subtract to RegA a ZP-memory + offset from RegX value
        SBC_AB  = 0xED, // 4 Cycles: Subtract to RegA a value from memory
        SBC_ABX = 0xFD, // 4 Cycles (+1 if cross boundary): Subtract to RegA a value from memory + offset from RegX
        SBC_ABY = 0xF9, // 4 Cycles (+1 if cross boundary): Subtract to RegA a value from memory + offset from RegY
        SBC_IDX = 0xE1, // 6 Cycles: Subtract to RegA a value from position pointed by [ZP-memory + offset from RegX]
        SBC_IDY = 0xF1, // 5 Cycles (+1 if cross boundary): Subtract to RegA a value from position ZP-memory + offset from RegY

        // >>>>> Logical <<<<<
        // > AND
        AND_IM  = 0x29, // 2 cycles: Use imediate value to AND with RegA
        AND_ZP  = 0x25, // 3 cycles: Use ZP-memory (0x0000 -> 0x00FF) to AND with RegA
        AND_ZPX = 0x35, // 4 cycles: Use ZP-memory + offset from RegX to AND with RegA
        AND_AB  = 0x2D, // 4 cycles: Use value from memory to AND with RegA
        AND_ABX = 0x3D, // 4 cycles: (+1 if cross boundary): Use value from memory + offset from RegX to AND with RegA
        AND_ABY = 0x39, // 4 cycles: (+1 if cross boundary): Use value from memory + offset from RegY to AND with RegA
        AND_IDX = 0x21, // 6 cycles: Use value from position pointed by [ZP-memory + offset from RegX] to AND with RegA
        AND_IDY = 0x31, // 5 cycles: (+1 if cross boundary): Use value from position ZP-memory + offset from RegY to AND with RegA

        // > ORA
        ORA_IM  = 0x09, // 2 cycles: Use imediate value to OR with RegA
        ORA_ZP  = 0x05, // 3 cycles: Use ZP-memory (0x0000 -> 0x00FF) to OR with RegA
        ORA_ZPX = 0x15, // 4 cycles: Use ZP-memory + offset from RegX to OR with RegA
        ORA_AB  = 0x0D, // 4 cycles: Use value from memory to OR with RegA
        ORA_ABX = 0x1D, // 4 cycles: (+1 if cross boundary): Use value from memory + offset from RegX to OR with RegA
        ORA_ABY = 0x19, // 4 cycles: (+1 if cross boundary): Use value from memory + offset from RegY to OR with RegA
        ORA_IDX = 0x01, // 6 cycles: Use value from position pointed by [ZP-memory + offset from RegX] to OR with RegA
        ORA_IDY = 0x11, // 5 cycles: (+1 if cross boundary): Use value from position ZP-memory + offset from RegY to OR with RegA

        // > EOR
        EOR_IM  = 0x49, // 2 cycles: Use imediate value to XOR with RegA
        EOR_ZP  = 0x45, // 3 cycles: Use ZP-memory (0x0000 -> 0x00FF) to XOR with RegA
        EOR_ZPX = 0x55, // 4 cycles: Use ZP-memory + offset from RegX to XOR with RegA
        EOR_AB  = 0x4D, // 4 cycles: Use value from memory to XOR with RegA
        EOR_ABX = 0x5D, // 4 cycles: (+1 if cross boundary): Use value from memory + offset from RegX to XOR with RegA
        EOR_ABY = 0x59, // 4 cycles: (+1 if cross boundary): Use value from memory + offset from RegY to XOR with RegA
        EOR_IDX = 0x41, // 6 cycles: Use value from position pointed by [ZP-memory + offset from RegX] to XOR with RegA
        EOR_IDY = 0x51, // 5 cycles: (+1 if cross boundary): Use value from position ZP-memory + offset from RegY to XOR with RegA

        // > BIT
        // Zero Flag: Bitwise AND value wiht RegA
        // Overflow Flag: Move bit 6 of value to flag
        // Negative Flag: Move bit 7 of value to flag
        BIT_ZP  = 0x24, // 3 cycles: Test bits in value from ZP memory and put it in Processor Flags
        BIT_AB  = 0x2C, // 4 cycles: Test bits in value from memory and put it in Processor Flags

        // > ASL
        ASL_RGA = 0x0A, // 2 cycles: Shifts 1 bit left in the RegA. Push 0 to the empty bit
        ASL_ZP  = 0x06, // 5 cycles: Shifts 1 bit left in ZP-memory address. Push 0 to the empty bit
        ASL_ZPX = 0x16, // 6 cycles: Shifts 1 bit left in ZP-memory address + offset from RegX. Push 0 to the empty bit
        ASL_AB  = 0x0E, // 6 cycles: Shifts 1 bit left in memory address. Push 0 to the empty bit
        ASL_ABX = 0x1E, // 7 cycles: Shifts 1 bit left in memory address + offset from RegX. Push 0 to the empty bit
        
        // > LSR
        LSR_RGA = 0x4A, // 2 cycles: Shifts 1 bit right in the RegA. Push 0 to the empty bit
        LSR_ZP  = 0x46, // 5 cycles: Shifts 1 bit right in ZP-memory address. Push 0 to the empty bit
        LSR_ZPX = 0x56, // 6 cycles: Shifts 1 bit right in ZP-memory address + offset from RegX. Push 0 to the empty bit
        LSR_AB  = 0x4E, // 6 cycles: Shifts 1 bit right in memory address. Push 0 to the empty bit
        LSR_ABX = 0x5E, // 7 cycles: Shifts 1 bit right in memory address + offset from RegX. Push 0 to the empty bit
        
        // > ROL
        ROL_RGA = 0x2A, // 2 cycles: Shifts 1 bit left in the RegA. Push Carry to the empty bit 
        ROL_ZP  = 0x26, // 5 cycles: Shifts 1 bit left in ZP-memory address. Push Carry to the empty bit  
        ROL_ZPX = 0x36, // 6 cycles: Shifts 1 bit left in ZP-memory address + offset from RegX.. Push Carry to the empty bit  
        ROL_AB  = 0x2E, // 6 cycles: Shifts 1 bit left in memory address. Push Carry to the empty bit  
        ROL_ABX = 0x3E, // 7 cycles: Shifts 1 bit left in memory address + offset from RegX. Push Carry to the empty bit  
        
        ROR_RGA = 0x6A, // 2 cycles: Shifts 1 bit right in the RegA. Push Carry to the empty bit 
        ROR_ZP  = 0x66, // 5 cycles: Shifts 1 bit right in ZP-memory address. Push Carry to the empty bit  
        ROR_ZPX = 0x76, // 6 cycles: Shifts 1 bit right in ZP-memory address + offset from RegX.. Push Carry to the empty bit  
        ROR_AB  = 0x6E, // 6 cycles: Shifts 1 bit right in memory address. Push Carry to the empty bit  
        ROR_ABX = 0x7E, // 7 cycles: Shifts 1 bit right in memory address + offset from RegX. Push Carry to the empty bit 

        // >>>>> Flags <<<<<
        // > Carry
        CLC     = 0x18, // 2 cycles: Clears the Carry Flag
        SEC     = 0x38, // 2 cycles: Sets the Carry Flag

        // > Decimal
        CLD     = 0xD8, // 2 cycles: Clears the Decimal Flag
        SED     = 0xF8, // 2 cycles: Sets the Decimal Flag

        // > Interrupt
        CLI     = 0x58, // 2 cycles: Clears the Interrupt Flag
        SEI     = 0x78, // 2 cycles: Sets the Interrupt Flag

        // > Overflow
        CLV     = 0xB8, // 2 cycles: Clears the Overflow Flag

        // >>>>> Register <<<<<
        TAX     = 0xAA, // 2 Cycles: Transfer RegA to RegX
        TAY     = 0xA8, // 2 Cycles: Transfer RegA to RegY
        TXA     = 0x8A, // 2 Cycles: Transfer RegX to RegA
        TYA     = 0x98, // 2 Cycles: Transfer RegY to RegA
        DEX     = 0xCA, // 2 Cycles: Decrement RegX
        DEY     = 0x88, // 2 Cycles: Decrement RegY
        INX     = 0xE8, // 2 Cycles: Increment RegX
        INY     = 0xC8, // 2 Cycles: Increment RegY

        // >>>>> Compare <<<<<
        // > CMP
        CMP_IM  = 0xC9, // 2 Cycles: Compare RegA to imediate value
        CMP_ZP  = 0xC5, // 3 Cycles: Compare RegA to value in ZP-memory
        CMP_ZPX = 0xD5, // 4 Cycles: Compare RegA to value in ZP-memory + RegX offset
        CMP_AB  = 0xCD, // 4 Cycles: Compare RegA to value in memory
        CMP_ABX = 0xDD, // 4 Cycles (+1 if cross boundary): Compare RegA to value in memory + RegX offset
        CMP_ABY = 0xD9, // 4 Cycles (+1 if cross boundary): Compare RegA to value in memory + RegX offset
        CMP_IDX = 0xC1, // 6 Cycles: Compare RegA to value in position pointed by [ZP-memory + RegX offset]
        CMP_IDY = 0xD1, // 5 Cycles (+1 if cross boundary): Compare RegA to value in position pointed by [ZP-memory] + RegY offset

        CPX_IM  = 0xE0, // 2 Cycles: Compare RegX to imediate value
        CPX_ZP  = 0xE4, // 3 Cycles: Compare RegX to ZP-memory value
        CPX_AB  = 0xEC, // 4 Cycles: Compare RegX to value in memory

        CPY_IM  = 0xC0, // 2 Cycles: Compare RegY to imediate value
        CPY_ZP  = 0xC4, // 3 Cycles: Compare RegY to ZP-memory value
        CPY_AB  = 0xCC, // 4 Cycles: Compare RegY to value in memory

        // >>>>> Branch <<<<<
        BPL     = 0x10, // 2 Cycles (+1 if it branches, +1 if cross boundary): Branches to address when Negative flag is clear
        BMI     = 0x30, // 2 Cycles (+1 if it branches, +1 if cross boundary): Branches to address when Negative flag is set
        BVC     = 0x50, // 2 Cycles (+1 if it branches, +1 if cross boundary): Branches to address when Overflow flag is clear
        BVS     = 0x70, // 2 Cycles (+1 if it branches, +1 if cross boundary): Branches to address when Overflow flag is set
        BCC     = 0x90, // 2 Cycles (+1 if it branches, +1 if cross boundary): Branches to address when Carry flag is clear
        BCS     = 0xB0, // 2 Cycles (+1 if it branches, +1 if cross boundary): Branches to address when Carry flag is set
        BNE     = 0xD0, // 2 Cycles (+1 if it branches, +1 if cross boundary): Branches to address when Zero flag is clear
        BEQ     = 0xF0, // 2 Cycles (+1 if it branches, +1 if cross boundary): Branches to address when Zero flag is set

        // >>>>> Stack <<<<<
        TSX     = 0xBA, // 2 cycles: Transfer Stack Pointer to Index X
        TXS     = 0x9A, // 2 cycles: Transfer Index X to Stack Register
        PHA     = 0x48, // 3 cycles: Push Accumulator on Stack
        PHP     = 0x08, // 3 cycles: Push Processor Status on Stack
                        // The status register will be pushed with the break flag and bit 5 set to 1
        PLA     = 0x68, // 4 cycles: Pull Accumulator from Stack
        PLP     = 0x28, // 4 cycles: Pull Processor Status from Stack
                        // The status register will be pulled with the break flag and bit 5 ignored        

        // >>>>> Special <<<<<
        BRK     = 0x00, // 7 cycles: Forces an interrupt of the CPU. Sets the break Flag
        RTI     = 0x40, // 6 cycles: Retrieves Program Counter and Processor Flags after an interrupt
        NOP     = 0xEA  // 2 cycles: Does nothing, just takes 2 cycles of CPU runtime
    };  
}

enum Variant : Byte
{
    CHIP_STANDART = 0,
    CHIP_65SC02 = 1
};

#endif