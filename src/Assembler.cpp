#include "Assembler_Class.h"

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

    InstructionSet opcodes;

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

        switch (mode)
        {
        case AddressMode::Immediate:
            command.append("_IM");
            break;
        case AddressMode::ZeroPage:
            command.append("_ZP");
            break;
        case AddressMode::ZeroPageX:
            command.append("_ZPX");
            break;
        case AddressMode::Absolute:
            command.append("_AB");
            break;
        case AddressMode::AbsoluteX:
            command.append("_ABX");
            break;
        case AddressMode::AbsoluteY:
            command.append("_ABY");
            break;
        case AddressMode::IndirectX:
            command.append("_IDX");
            break;
        case AddressMode::IndirectY:
            command.append("_IDY");
            break;
        default: break;
        }

        if(opcodes.find(command) == opcodes.end())
        {
            std::cout << "ERROR: Invalid argument for instruction!" << std::endl;
            CloseFiles(inFile, outFile);
            return 2;
        }

        Byte instruction = opcodes[command];
        outFile.write((char*) &instruction, sizeof(Byte));
        if(value > 255)
            outFile.write((char*) &value, sizeof(Word));
        else
            outFile.write((char*) &value, sizeof(Byte));
    }

    CloseFiles(inFile, outFile);
    return 0;
}