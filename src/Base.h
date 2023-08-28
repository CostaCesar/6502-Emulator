#ifndef BASE_H
#define BASE_H

using Byte = unsigned char;
using Word = unsigned short;

/* Opcodes */
enum Instruction: Byte
{
    INS_LDA_IM = 0xA9,  // 2 cycles: Load To RegA Imediate value
    INS_LDA_ZP = 0xA5,  // 3 cycles: Load to RegA value from memory
    INS_LDA_ZPX = 0xB5, // 4 cycles: Load to RegA value from memory + offset from RegX
    INS_JSR = 0x20      // 6 cycles: Branch to subroutine
};

#endif