#include "Assembler_Class.h"
#include "Base.h"

#define BYTE_PATTERN "\\$([0-9a-fA-F]{1,2})|%([01]{1,8})|(?:(25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[0-9]{2}|[0-9]))"
#define WORD_PATTERN "\\$([0-9a-fA-F]{1,4})|%([01]{1,16})|(?:(6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{4}|[0-9]{1,4}))"

using std::string;
typedef std::unordered_map<AddressMode, std::regex> AddressMap;

bool ModeUsesWord(AddressMode mode)
{
    return (mode == AddressMode::Absolute || mode == AddressMode::AbsoluteX 
        || mode == AddressMode::AbsoluteY);
}
AddressMap LoadAddressRegex()
{
    return AddressMap
    {
        {AddressMode::Absolute, std::regex(" (" WORD_PATTERN ") *$")},
        {AddressMode::AbsoluteX, std::regex(" (" WORD_PATTERN "), *[Xx] *$")},
        {AddressMode::AbsoluteY, std::regex(" (" WORD_PATTERN "), *[Yy] *$")},
        {AddressMode::IndirectX, std::regex(" \\((" WORD_PATTERN ", *[Xx])\\) *$")},
        {AddressMode::IndirectY, std::regex(" \\((" WORD_PATTERN ")\\), *[Yy] *$")},
        {AddressMode::ZeroPage, std::regex(" (" BYTE_PATTERN ") *$")},
        {AddressMode::ZeroPageX, std::regex(" (" BYTE_PATTERN "), *[Xx] *$")},
        {AddressMode::ZeroPageY, std::regex(" (" BYTE_PATTERN "), *[Yy] *$")},
        {AddressMode::Immediate, std::regex(" #(" BYTE_PATTERN ") *$")},
        {AddressMode::Acumulator, std::regex(" ([Aa]) *$")},
        {AddressMode::Default, std::regex("[A-Za-z]{3} *$")}
    };
}

Word ConvertFromBase(string value, Byte base)
{
    Word output = 0;

    for (size_t i = 0 ; i < value.length(); i++)
    {
        // Handling bases with letters
        if(value[i] >= 'a' && value[i] <= 'f')
            output = (output * base) + (value[i] - 87);
        else if(value[i] >= 'A' && value[i] <= 'F')
            output = (output * base) + (value[i] - 55);
        // All bases from 10 below
        else if(isdigit(value[i]))
            output = (output * base) + (value[i] - '0');
        else break;
    }
    
    return output;
}

uint8_t OpenFile_In(std::ifstream &inFile, const char *inFilePath)
{
    inFile.open(inFilePath);
    if(!inFile.is_open())
    {
        std::cout << "ERROR: Could not open file \"" << inFilePath << "\"" << std::endl;
        return 1;
    }
    
    return 0;
}
uint8_t OpenFile_Out(std::ofstream& outFile, const char *outFilePath)
{
    if(std::filesystem::exists(outFilePath))
    {   
        std::cout << "WARNING: A file with the name \"" << outFilePath << "\" already exists! Overwrite [y/N]? " << std::endl;
        
        char command = getchar();
        if (command == 'n' || command == 'N' || command == '\n')
            return 1;
    }
    outFile.open(outFilePath, std::ios::binary);
    if(!outFile.is_open())
    {
        std::cout << "ERROR: Could not create file \"" << outFilePath << "\"" << std::endl;
        return 1;
    }

    return 0;
}
void CloseFiles(std::ifstream &inFile, std::ofstream &outFile)
{
    if(inFile.is_open())
        inFile.close();
    if(outFile.is_open())
        outFile.close();
    return;
}

uint8_t GetArgs(int argc, char** argv, std::string &outFilePath)
{
    if(argc < 2)
    {
        std::cout << "ERROR: No input file specified!" << std::endl;
        return 1;
    }
    
    if(argc == 3) outFilePath = argv[2];
    else outFilePath = "a.bin";

    return 0;
}

void RemoveComment(std::string &line)
{
    line = line.substr(0, line.find(';'));
    return;
}

uint8_t GetCommand(const std::string &line, std::string &command)
{
    if(line.empty())
        return 1;
        
    std::regex command_pattern("([A-Za-z]{3}) ");
    std::smatch result;
    
    std::regex_search(line, result, command_pattern);
    command = result[1].str();

    return 0;
}
uint8_t ApplyMode(std::string &command, AddressMode mode, InstructionSet& opcodes)
{
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
    case AddressMode::ZeroPageY:
        command.append("_ZPY");
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
    case AddressMode::Acumulator:
        command.append("_RGA");
        break;
    default:
        return 1;
    }

    if(opcodes.exists(command) == false) return 2;
    else return 0;
}

uint8_t EvalueParam(const std::string &line, Word &value, AddressMode &mode, const AddressMap &addressTable)
{
    std::smatch result;
    for (auto const &pattern : addressTable)
    {

        std::regex_search(line, result, pattern.second);
        
        // Result 0 return the entire match
        // Result 1 return the value and, if included, the base operand
        // Result 2 return the value only
        if(result[0].matched)
        {
            mode = pattern.first;
            switch (result[1].str()[0])
            {
            case '$':
                value = ConvertFromBase(result[2].str(), 16);
                break;
            case '%':
                value = ConvertFromBase(result[3].str(), 2);
                break;
            default:
                value = ConvertFromBase(result[4].str(), 10);
            }
            return 0;
        }
    }
    
    return 1;
}
