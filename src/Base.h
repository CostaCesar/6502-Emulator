#ifndef BASE_H
#define BASE_H

using Byte = unsigned char;
using Word = unsigned short;

/* Opcodes */
enum Instruction: Byte
{
    // >>>>> Memory <<<<<
    // > LDA
    INS_LDA_IM  = 0xA9, // 2 cycles: Load to RegA imediate value
    INS_LDA_ZP  = 0xA5, // 3 cycles: Load to RegA value from ZP-memory (0x0000 -> 0x00FF)
    INS_LDA_ZPX = 0xB5, // 4 cycles: Load to RegA value from ZP-memory + offset from RegX
    INS_LDA_AB  = 0xAD, // 4 cycles: Load to RegA value from memory
    INS_LDA_ABX = 0xBD, // 4 cycles (+1 if cross boundary): Load to RegA value from memory + offset from RegX
    INS_LDA_ABY = 0xB9, // 4 cycles (+1 if cross boundary): Load to RegA value from memory + offset from RegY
    INS_LDA_IDX = 0xA1, // 6 cycles : Load to RegA value from position pointed by [ZP-memory + offset from RegX]
    INS_LDA_IDY = 0xB1, // 5 cycles (+1 if cross boundary): Load to RegA value from position ZP-memory + offset from RegY
    
    // > LDX
    INS_LDX_IM  = 0xA0, // 2 cycles: Load to RegX imediate value
    INS_LDX_ZP  = 0xA4, // 3 cycles: Load to RegX value from ZP-memory (0x0000 -> 0x00FF)
    INS_LDX_ZPY = 0xB4, // 4 cycles: Load to RegX value from ZP-memory + offset from RegY
    INS_LDX_AB  = 0xAC, // 4 cycles: Load to RegX value from memory
    INS_LDX_ABY = 0xBC, // 4 cycles (+1 if cross boundary): Load to RegX value from memory + offset from RegY

    // > LDY
    INS_LDY_IM  = 0xA2, // 2 cycles: Load to RegY imediate value
    INS_LDY_ZP  = 0xA6, // 3 cycles: Load to RegY value from ZP-memory (0x0000 -> 0x00FF)
    INS_LDY_ZPX = 0xB6, // 4 cycles: Load to RegY value from ZP-memory + offset from RegX
    INS_LDY_AB  = 0xAE, // 4 cycles: Load to RegY value from memory
    INS_LDY_ABX = 0xBE, // 4 cycles (+1 if cross boundary): Load to RegY value from memory + offset from RegX

    // > STA
    INS_STA_ZP  = 0x85, // 3 cycles: Store RegA value to ZP-memory (0x0000 -> 0x00FF)
    INS_STA_ZPX = 0x95, // 4 cycles: Store RegA value to ZP-memory + offset from RegX
    INS_STA_AB  = 0x8D, // 4 cycles: Store RegA value to memory
    INS_STA_ABX = 0x9D, // 5 cycles: Store RegA value to memory + offset from RegX
    INS_STA_ABY = 0x99, // 5 cycles: Store RegA value tomemory + offset from RegY
    INS_STA_IDX = 0x81, // 6 cycles: Store RegA value to position pointed by [ZP-memory + offset from RegX]
    INS_STA_IDY = 0x91, // 6 cycles: Store RegA value to position ZP-memory + offset from RegY

    // > STX
    INS_STX_ZP  = 0x86, // 3 cycles: Store RegX value to ZP-memory (0x0000 -> 0x00FF)
    INS_STX_ZPY = 0x96, // 4 cycles: Store RegX value to ZP-memory + offset from RegY
    INS_STX_AB  = 0x8E, // 4 cycles: Store RegX value to memory

    // > STY
    INS_STY_ZP  = 0x84, // 3 cycles: Store RegY value to ZP-memory (0x0000 -> 0x00FF)
    INS_STY_ZPX = 0x94, // 4 cycles: Store RegY value to ZP-memory + offset from RegX
    INS_STY_AB  = 0x8C, // 4 cycles: Store RegY value to memory
    
    // >>>>> Jump <<<<<
    INS_JSR = 0x20,     // 6 cycles: Branch to subroutine
    INS_RTS = 0x60      // 6 cycles: Return from subroutine 
};

#endif