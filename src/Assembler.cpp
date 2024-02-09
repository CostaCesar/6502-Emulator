#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <map>
#include "Base.h"

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

Word ConvertFromBase(string value, Byte base)
{
    Word output = 0;

    for (size_t i = 0 ; i < value.length(); i++)
    {
        if(!isdigit(value[i]))
            break;
        output = (output * base) + (value[i] - '0');
    }
    
    return output;
}

void LoadInstructions(std::map<string, std::map<AddressMode::Mode, Byte>> &opcodes)
{
    opcodes["LDA"][AddressMode::Immediate] = 0xA9; 
    opcodes["LDA"][AddressMode::ZeroPage ] = 0xA5; 
    opcodes["LDA"][AddressMode::ZeroPageX] = 0xB5; 
    opcodes["LDA"][AddressMode::Absolute ] = 0xAD; 
    opcodes["LDA"][AddressMode::AbsoluteX] = 0xBD; 
    opcodes["LDA"][AddressMode::AbsoluteY] = 0xB9; 
    opcodes["LDA"][AddressMode::IndirectX] = 0xA1; 
    opcodes["LDA"][AddressMode::IndirectY] = 0xB1; 
    opcodes["LDX"][AddressMode::Immediate] = 0xA0; 
    opcodes["LDX"][AddressMode::ZeroPage ] = 0xA4; 
    opcodes["LDX"][AddressMode::ZeroPageY] = 0xB4; 
    opcodes["LDX"][AddressMode::Absolute ] = 0xAC; 
    opcodes["LDX"][AddressMode::AbsoluteY] = 0xBC; 
    opcodes["LDY"][AddressMode::Immediate] = 0xA2;
    opcodes["LDY"][AddressMode::ZeroPage ] = 0xA6; 
    opcodes["LDY"][AddressMode::ZeroPageX] = 0xB6;
    opcodes["LDY"][AddressMode::Absolute ] = 0xAE; 
    opcodes["LDY"][AddressMode::AbsoluteX] = 0xBE; 
    opcodes["STA"][AddressMode::ZeroPage ] = 0x85; 
    opcodes["STA"][AddressMode::ZeroPageX] = 0x95;
    opcodes["STA"][AddressMode::Absolute ] = 0x8D; 
    opcodes["STA"][AddressMode::AbsoluteX] = 0x9D; 
    opcodes["STA"][AddressMode::AbsoluteY] = 0x99;
    opcodes["STA"][AddressMode::IndirectX] = 0x81; 
    opcodes["STA"][AddressMode::IndirectY] = 0x91; 
    opcodes["STX"][AddressMode::ZeroPage ] = 0x86; 
    opcodes["STX"][AddressMode::ZeroPageY] = 0x96;
    opcodes["STX"][AddressMode::Absolute ] = 0x8E; 
    opcodes["STY"][AddressMode::ZeroPage ] = 0x84; 
    opcodes["STY"][AddressMode::ZeroPageX] = 0x94;
    opcodes["STY"][AddressMode::Absolute ] = 0x8C; 
    opcodes["DEC"][AddressMode::ZeroPage ] = 0xC6; 
    opcodes["DEC"][AddressMode::ZeroPageX] = 0xD6; 
    opcodes["DEC"][AddressMode::Absolute ] = 0xCE;
    opcodes["DEC"][AddressMode::AbsoluteX] = 0xDE; 
    opcodes["INC"][AddressMode::ZeroPage ] = 0xE6; 
    opcodes["INC"][AddressMode::ZeroPageX] = 0xF6; 
    opcodes["INC"][AddressMode::Absolute ] = 0xEE;
    opcodes["INC"][AddressMode::AbsoluteX] = 0xFE; 
    opcodes["JSR"][AddressMode::Default  ] = 0x20;
    opcodes["RTS"][AddressMode::Default  ] = 0x60;
    opcodes["JMP"][AddressMode::Absolute ] = 0x4C; 
    opcodes["JMP"][AddressMode::Absolute ] = 0x6C; 
    opcodes["ADC"][AddressMode::Immediate] = 0x69; 
    opcodes["ADC"][AddressMode::ZeroPage ] = 0x65;
    opcodes["ADC"][AddressMode::ZeroPageX] = 0x75; 
    opcodes["ADC"][AddressMode::Absolute ] = 0x6D;
    opcodes["ADC"][AddressMode::AbsoluteX] = 0x7D; 
    opcodes["ADC"][AddressMode::AbsoluteY] = 0x79; 
    opcodes["ADC"][AddressMode::IndirectX] = 0x61; 
    opcodes["ADC"][AddressMode::IndirectY] = 0x71;
    opcodes["SBC"][AddressMode::Immediate] = 0xE9;
    opcodes["SBC"][AddressMode::ZeroPage ] = 0xE5; 
    opcodes["SBC"][AddressMode::ZeroPageX] = 0xF5; 
    opcodes["SBC"][AddressMode::Absolute ] = 0xED; 
    opcodes["SBC"][AddressMode::AbsoluteX] = 0xFD; 
    opcodes["SBC"][AddressMode::AbsoluteY] = 0xF9; 
    opcodes["SBC"][AddressMode::IndirectX] = 0xE1; 
    opcodes["SBC"][AddressMode::IndirectY] = 0xF1; 
    opcodes["AND"][AddressMode::Immediate] = 0x29; 
    opcodes["AND"][AddressMode::ZeroPage ] = 0x25; 
    opcodes["AND"][AddressMode::ZeroPageX] = 0x35;
    opcodes["AND"][AddressMode::Absolute ] = 0x2D;
    opcodes["AND"][AddressMode::AbsoluteX] = 0x3D;
    opcodes["AND"][AddressMode::AbsoluteY] = 0x39;
    opcodes["AND"][AddressMode::IndirectX] = 0x21;
    opcodes["AND"][AddressMode::IndirectY] = 0x31;
    opcodes["ORA"][AddressMode::Immediate] = 0x09; 
    opcodes["ORA"][AddressMode::ZeroPage ] = 0x05; 
    opcodes["ORA"][AddressMode::ZeroPageX] = 0x15;
    opcodes["ORA"][AddressMode::Absolute ] = 0x0D; 
    opcodes["ORA"][AddressMode::AbsoluteX] = 0x1D;
    opcodes["ORA"][AddressMode::AbsoluteY] = 0x19;
    opcodes["ORA"][AddressMode::IndirectX] = 0x01; 
    opcodes["ORA"][AddressMode::IndirectY] = 0x11; 
    opcodes["EOR"][AddressMode::Immediate] = 0x49;
    opcodes["EOR"][AddressMode::ZeroPage ] = 0x45; 
    opcodes["EOR"][AddressMode::ZeroPageX] = 0x55; 
    opcodes["EOR"][AddressMode::Absolute ] = 0x4D;
    opcodes["EOR"][AddressMode::AbsoluteX] = 0x5D;
    opcodes["EOR"][AddressMode::AbsoluteY] = 0x59;
    opcodes["EOR"][AddressMode::IndirectX] = 0x41;
    opcodes["EOR"][AddressMode::IndirectY] = 0x51;
    opcodes["BIT"][AddressMode::ZeroPage ] = 0x24;
    opcodes["BIT"][AddressMode::Absolute ] = 0x2C;
    opcodes["ASL"][AddressMode::Default  ] = 0x0A;
    opcodes["ASL"][AddressMode::ZeroPage ] = 0x06; 
    opcodes["ASL"][AddressMode::ZeroPageX] = 0x16; 
    opcodes["ASL"][AddressMode::Absolute ] = 0x0E; 
    opcodes["ASL"][AddressMode::AbsoluteX] = 0x1E; 
    opcodes["LSR"][AddressMode::Default  ] = 0x4A; 
    opcodes["LSR"][AddressMode::ZeroPage ] = 0x46; 
    opcodes["LSR"][AddressMode::ZeroPageX] = 0x56; 
    opcodes["LSR"][AddressMode::Absolute ] = 0x4E; 
    opcodes["LSR"][AddressMode::AbsoluteX] = 0x5E;
    opcodes["ROL"][AddressMode::Default  ] = 0x2A; 
    opcodes["ROL"][AddressMode::ZeroPage ] = 0x26; 
    opcodes["ROL"][AddressMode::ZeroPageX] = 0x36;  
    opcodes["ROL"][AddressMode::Absolute ] = 0x2E; 
    opcodes["ROL"][AddressMode::AbsoluteX] = 0x3E; 
    opcodes["ROR"][AddressMode::Default  ] = 0x6A; 
    opcodes["ROR"][AddressMode::ZeroPage ] = 0x66;   
    opcodes["ROR"][AddressMode::ZeroPageX] = 0x76; 
    opcodes["ROR"][AddressMode::Absolute ] = 0x6E; 
    opcodes["ROR"][AddressMode::AbsoluteX] = 0x7E; 
    opcodes["TSX"][AddressMode::Default  ] = 0xBA;
    opcodes["TXS"][AddressMode::Default  ] = 0x9A;
    opcodes["PHA"][AddressMode::Default  ] = 0x48; 
    opcodes["PHP"][AddressMode::Default  ] = 0x08; 
    opcodes["PLA"][AddressMode::Default  ] = 0x68; 
    opcodes["PLP"][AddressMode::Default  ] = 0x28; 
    opcodes["BRK"][AddressMode::Default  ] = 0x00;
    opcodes["RTI"][AddressMode::Default  ] = 0x40;
    opcodes["NOP"][AddressMode::Default  ] = 0xEA;

    return;
}

void CloseFiles(std::ifstream &inFile, std::ofstream &outFile)
{
    if(inFile.is_open())
        inFile.close();
    if(outFile.is_open())
        outFile.close();
    return;
}

int main(int argc, char ** argv)
{
    string line;
    string command;
    
    if(argc < 2)
    {
        std::cout << "ERROR: No input file specified!" << std::endl;
        return 1;
    }
    if(argc == 3) line = argv[2];
    else line = "a.bin";
    
    std::ifstream inFile(argv[1]);
    if(!inFile.is_open())
    {
        std::cout << "ERROR: Could not open file \"" << argv[1] << "\"" << std::endl;
        return 1;
    }

    if(std::filesystem::exists(line))
    {   
        std::cout << "WARNING: A file with the name \"" << line << "\" already exists! Overwrite [y/N]? " << std::endl;
        command = getchar();
        if (command == "n" || command == "N" || command == "\n")
        {
            inFile.close();
            return 0;
        }
        
    }
    std::ofstream outFile(line, std::ios::binary);
    if(!outFile.is_open())
    {
        std::cout << "ERROR: Could not create file \"" << argv[2] << "\"" << std::endl;
        CloseFiles(inFile, outFile);
        return 1;
    }

    std::map<string, std::map<AddressMode::Mode, Byte>> opcodes;
    LoadInstructions(opcodes);

    while(getline(inFile, line))
    {
        AddressMode::Mode mode = AddressMode::Default;
        NumberMode::Mode base = NumberMode::Decimal;
        Word value = 0;
        size_t atChar = 0;
        
        line = line.substr(0, line.find(';'));
        if(line.empty())
            continue;
        
        command = line.substr(0, line.find_first_of(' '));
        line = line.substr(line.find_first_of(' ') + 1, line.length()-1) + '*';

        if(opcodes.find(command) == opcodes.end())
        {
            std::cout << "ERROR: Invalid command!" << std::endl;
            CloseFiles(inFile, outFile);
            return 2;
        }
        
        if(line.empty())
            continue;

        if(line[atChar] == '#')
        {
            mode = AddressMode::Immediate;
            atChar++;
        }
        else if(line[atChar] == '(')
        {
            if(line.find('X') != string::npos)
                mode = AddressMode::IndirectX;
            else if(line.find('Y') != string::npos)
                mode = AddressMode::IndirectY;
            else if(command.compare("JMP")) // (JMP only)
                mode = AddressMode::Absolute;
            else
            {
                std::cout << "ERROR: Invalid argument!" << std::endl;
                CloseFiles(inFile, outFile);
                return 2;
            };
            atChar++;
        }
        
        switch (line[atChar])
        {
        case '%':
            base = NumberMode::Binary;
            atChar++;
            break;
        // Implement Octal base in the future
        case '$':
            base = NumberMode::HexaDec;
            atChar++;
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            base = NumberMode::Decimal;
            break;
        default:
            std::cout << "ERROR: Invalid argument!" << std::endl;
            CloseFiles(inFile, outFile);
            return 2;
        }

        value = ConvertFromBase(line.substr(atChar, line.length()), base);
        if(mode == AddressMode::Default)
        {
            if(value < 256) // ZeroPage (Byte)
            {
                if(line.find(',') == string::npos)
                    mode = AddressMode::ZeroPage;
                else if(line.find('X') != string::npos)
                    mode = AddressMode::ZeroPageX;
            }
            else // Absolute (Word)
            {
                if(line.find(',') == string::npos)
                    mode = AddressMode::Absolute;
                else if(line.find('X') != string::npos)
                    mode = AddressMode::AbsoluteX;
                else if(line.find('Y') != string::npos)
                    mode = AddressMode::AbsoluteY;
            }
        }

        if(opcodes[command].find((AddressMode::Mode) mode) == opcodes[command].end())
        {
            std::cout << "ERROR: Invalid argument for instruction!" << std::endl;
            CloseFiles(inFile, outFile);
            return 2;
        }

        outFile.write((char*) &opcodes[command][(AddressMode::Mode) mode], sizeof(Byte));
        if(value > 255)
            outFile.write((char*) &value, sizeof(Word));
        else
            outFile.write((char*) &value, sizeof(Byte));
    }
    CloseFiles(inFile, outFile);
    return 0;
}