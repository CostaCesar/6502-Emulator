#include "Assembler_Class.h"

int main(int argc, char ** argv)
{
    string line, command;
    if(GetArgs(argc, argv, line) != 0) return 1;
    
    std::ifstream inFile;
    std::ofstream outFile;

    if(OpenFile_In(inFile, argv[1]) != 0 || OpenFile_Out(outFile, line.c_str()))
    {
        CloseFiles(inFile, outFile);
        return 1;
    }
    
    InstructionSet opcodes;
    std::unordered_map<AddressMode, std::regex> addressTable = LoadAddressRegex();

    for(size_t line_count = 1; getline(inFile, line); line_count++)
    {
        AddressMode mode = AddressMode::Default;
        Word value = 0;
        
        RemoveComment(line);
        if(GetCommand(line, command) != 0)
            continue;

        if(opcodes.find(command) == opcodes.end())
        {
            std::cout << "Line " << std::to_string(line_count) << std::endl;
            std::cout << "ERROR: Invalid command!" << std::endl;
            CloseFiles(inFile, outFile);
            return 2;
        }

        if(EvalueParam(line, value, mode, addressTable) != 0)
        {
            std::cout << "Line " << std::to_string(line_count) << std::endl;
            std::cout << "ERROR: Invalid argumuments!" << std::endl;
            CloseFiles(inFile, outFile);
            return 2;
        };

        if(ApplyMode(command, mode, opcodes) != 0)
        {
            std::cout << "Line " << std::to_string(line_count) << std::endl;
            std::cout << "ERROR: Invalid format for the command \"" << command << "\"" << std::endl;
            CloseFiles(inFile, outFile);
            return 2;
        }

        Byte command_opcode = opcodes[command];

        outFile.write((char*) &command_opcode, sizeof(Byte));
        if(value > 255 || ModeUsesWord(mode) == true)
            outFile.write((char*) &value, sizeof(Word));
        else
            outFile.write((char*) &value, sizeof(Byte));
        
    }

    CloseFiles(inFile, outFile);
    return 0;
}