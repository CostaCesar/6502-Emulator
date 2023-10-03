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
        LDA_ABX = 0xBD, // 4 cycles (+1 if cross boundary): Load to RegA value from memory + offset from RegX
        LDA_ABY = 0xB9, // 4 cycles (+1 if cross boundary): Load to RegA value from memory + offset from RegY
        LDA_IDX = 0xA1, // 6 cycles : Load to RegA value from position pointed by [ZP-memory + offset from RegX]
        LDA_IDY = 0xB1, // 5 cycles (+1 if cross boundary): Load to RegA value from position ZP-memory + offset from RegY
        
        // > LDX
        LDX_IM  = 0xA0, // 2 cycles: Load to RegX imediate value
        LDX_ZP  = 0xA4, // 3 cycles: Load to RegX value from ZP-memory (0x0000 -> 0x00FF)
        LDX_ZPY = 0xB4, // 4 cycles: Load to RegX value from ZP-memory + offset from RegY
        LDX_AB  = 0xAC, // 4 cycles: Load to RegX value from memory
        LDX_ABY = 0xBC, // 4 cycles (+1 if cross boundary): Load to RegX value from memory + offset from RegY

        // > LDY
        LDY_IM  = 0xA2, // 2 cycles: Load to RegY imediate value
        LDY_ZP  = 0xA6, // 3 cycles: Load to RegY value from ZP-memory (0x0000 -> 0x00FF)
        LDY_ZPX = 0xB6, // 4 cycles: Load to RegY value from ZP-memory + offset from RegX
        LDY_AB  = 0xAE, // 4 cycles: Load to RegY value from memory
        LDY_ABX = 0xBE, // 4 cycles (+1 if cross boundary): Load to RegY value from memory + offset from RegX

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
        JSR     = 0x20,     // 6 cycles: Branch to subroutine
        RTS     = 0x60,     // 6 cycles: Return from subroutine
        JMP_AB  = 0x4C,     // 3 cycles: Jump to position in memory directly
        JMP_ID  = 0x6C      // 3 cycles: Jump to position in memory pointed by another position 
    };  
}

enum Variant : Byte
{
    CHIP_STANDART = 1,
    CHIP_65SC02 = 2
};

#endif