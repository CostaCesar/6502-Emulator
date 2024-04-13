#ifndef BASE_H
#define BASE_H

using Byte = unsigned char;
using Word = unsigned short;

enum Variant : Byte
{
    CHIP_STANDART = 0,
    CHIP_65SC02 = 1
};

#endif