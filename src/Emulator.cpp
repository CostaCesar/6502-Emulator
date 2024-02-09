#include <fstream>
#include <iostream>
#include "Base.h"
#include "CPU.h"
#include "Memory.h"

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cout << "ERROR: No input file specified!" << std::endl;
        return 1;
    }
    
    std::ifstream inFile(argv[1], std::ios::binary);
    if(!inFile.is_open())
    {
        std::cout << "ERROR: Could not open file \"" << argv[1] << "\"" << std::endl;
        return 1;
    }

    Variant Chip = CHIP_STANDART;
    if(argc < 3) std::cout << "WARNING: No chip variant specified! Assuming base chip [6502]" << std::endl;
    else Chip = (Variant) (argv[2][0] - '0');

    Memory Ram;
    Word Address = 0x1000;

    Ram.Initialise();
    CPU Processor(Chip, Address);

    for(; inFile.peek() != EOF; Address++)
    {
        inFile.read((char *) &Ram[Address], sizeof(Byte));
    }
    inFile.close();

    Processor.Execute(100, Ram);

    return 0;
}
