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
    uMap_ModeToRegex addressTable = LoadAddressRegex();
    uMap_LabelToPos labelTable;
    uMap_PosToLabel labelReferences;

    for(Word line_count = 1, memoryPos = 0; getline(inFile, line); line_count++)
    {
        AddressMode mode = AddressMode::Default;
        Word value = 0;
        
        if(RemoveComment(line) != 0)
            continue;
        
        if(GetLabel(line, command) == true)
            labelTable[command] = memoryPos;
        if(GetCommand(line, command) != 0)
            continue;

        if(opcodes.find(command) == opcodes.end())
        {
            std::cout << "Line " << std::to_string(line_count) << std::endl;
            std::cout << "ERROR: Invalid command!" << std::endl;
            CloseFiles(inFile, outFile);
            DeleteFile(argv[2]);
            return 2;
        }

        mode = GetMode_Label(command);
        if(EvalueLabel(line, memoryPos, labelReferences, mode != AddressMode::Relative) == 0);
        else if(EvalueParam(line, value, mode, addressTable) != 0)
        {
            std::cout << "Line " << std::to_string(line_count) << std::endl;
            std::cout << "ERROR: Invalid argumuments!" << std::endl;
            CloseFiles(inFile, outFile);
            DeleteFile(argv[2]);
            return 2;
        };

        if(ApplyMode(command, mode, opcodes) != 0)
        {
            std::cout << "Line " << std::to_string(line_count) << std::endl;
            std::cout << "ERROR: Invalid format for the command \"" << command << "\"" << std::endl;
            CloseFiles(inFile, outFile);
            DeleteFile(argv[2]);
            return 2;
        }

        Byte command_opcode = opcodes[command];

        outFile.write((char*) &command_opcode, sizeof(Byte));
        memoryPos++;
        if(mode == AddressMode::Default) continue;

        if(value > 255 || ModeUsesWord(mode) == true)
        {   // Write word
            outFile.write((char*) &value, sizeof(Word));
            memoryPos += 2;
        }
        else 
        {   // Write byte
            outFile.write((char*) &value, sizeof(Byte));
            memoryPos++;
        } 
    }

    if(ReplaceLabels(labelReferences, labelTable, outFile) != 0)
    {
        std::cout << "ERROR: Non-declared label found!" << std::endl;
        CloseFiles(inFile, outFile);
        DeleteFile(argv[2]);
        return 2;
    }

    CloseFiles(inFile, outFile);
    return 0;
}