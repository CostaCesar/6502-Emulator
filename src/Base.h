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
        
        // >>>>> Jump <<<<<
        JSR     = 0x20, // 6 cycles: Branch to subroutine
        RTS     = 0x60, // 6 cycles: Return from subroutine
        JMP_AB  = 0x4C, // 3 cycles: Jump to position in memory directly
        JMP_ID  = 0x6C, // 3 cycles: Jump to position in memory pointed by another position

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

        // >>>>> Stack <<<<<
        TSX     = 0xBA, // 2 cycles: Transfer Stack Pointer to Index X
        TXS     = 0x9A, // 2 cycles: Transfer Index X to Stack Register
        PHA     = 0x48, // 3 cycles: Push Accumulator on Stack
        PHP     = 0x08, // 3 cycles: Push Processor Status on Stack
                        // The status register will be pushed with the break flag and bit 5 set to 1.
        PLA     = 0x68, // 4 cycles: Pull Accumulator from Stack
        PLP     = 0x28  // 4 cycles: Pull Processor Status from Stack
                        // The status register will be pulled with the break flag and bit 5 ignored
                            
    };  
}

enum Variant : Byte
{
    CHIP_STANDART = 1,
    CHIP_65SC02 = 2
};

#endif