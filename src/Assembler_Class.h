#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "Base.h"
#include "Instructions.h"

using std::string;

namespace NumberMode
{
    enum Mode: Byte
    {
        HexaDec = 16,
        Decimal = 10,
        Octal   = 8,
        Binary  = 2
    };
}
namespace AddressMode
{
    enum Mode: Byte
    {
        Immediate  = 0,
        ZeroPage   = 1,
        ZeroPageX  = 2,
        ZeroPageY  = 3,
        Absolute   = 4,
        AbsoluteX  = 5,
        AbsoluteY  = 6,
        IndirectX  = 7,
        IndirectY  = 8,
        Default    = 10,
    };
}

Word ConvertFromBase(string value, Byte base);
void CloseFiles(std::ifstream &inFile, std::ofstream &outFile);