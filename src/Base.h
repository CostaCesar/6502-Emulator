#ifndef BASE_H
#define BASE_H

using Byte = unsigned char;
using Word = unsigned short;

/* Opcodes */
enum Instruction: Byte
{
    // Memory
    // > LDA
    INS_LDA_IM = 0xA9,  // 2 cycles: Load to RegA imediate value
    INS_LDA_ZP = 0xA5,  // 3 cycles: Load to RegA value from ZP-memory (0x0000 -> 0x00FF)
    INS_LDA_ZPX = 0xB5, // 4 cycles: Load to RegA value from ZP-memory + offset from RegX
    INS_LDA_AB = 0xAD,  // 4 cycles: Load to RegA value from memory
    INS_LDA_ABX = 0xBD, // 4 cycles (+1 if cross boundary): Load to RegA value from memory + offset from RegX
    INS_LDA_ABY = 0xB9, // 4 cycles (+1 if cross boundary): Load to RegA value from memory + offset from RegY
    INS_LDA_IDX = 0xA1, // 6 cycles : Load to RegA value from position pointed by [ZP-memory + offset from RegX]
    INS_LDA_IDY = 0xB1, // 5 cycles (+1 if cross boundary): Load to RegA value from position ZP-memory + offset from RegY
    
    // Jump
    INS_JSR = 0x20      // 6 cycles: Branch to subroutine
};

#endif