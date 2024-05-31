#include "Assembler.h"
#include "Base.h"

using std::string;
using namespace AssemblerDefs;

#define BYTE_PATTERN "\\$([0-9a-fA-F]{1,2})|%([01]{1,8})|(?:(25[0-5]|2[0-4][0-9]|[0-1][0-9]{2}|[0-9]{2}|[0-9]))"
#define WORD_PATTERN "\\$([0-9a-fA-F]{1,4})|%([01]{1,16})|(?:(6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[1-5][0-9]{4}|[0-9]{1,4}))"
#define LABEL_PATTERN "^([A-Za-z]{1}[A-Za-z0-9_]*): *$"
#define LABEL_REF_PATTERN "([A-Za-z]{1}[A-Za-z0-9_]*) *$"
#define INSTRUCTION_PATTERN "^ *([A-Za-z]{3,3})( +|$)"

Assembler::Assembler()
{
    modeTable = LoadAddressRegex();
    lineCount = memoryPos = 0;
}
uint8_t Assembler::WriteBuffer(std::ofstream &outPath)
{
    outPath.write((char *) buffer.data(), buffer.size() * sizeof(Byte));
    if(outPath.bad())
    {
        std::cout << "ERROR: Could not write to file" << std::endl;
        return 1;
    }
    return 0;
}

uint8_t Assembler::CompileLine(string line)
{
    AddressMode mode = AddressMode::Default;
    string command = "";
    Word value = 0;
    
    if (RemoveComment(line) != 0)
        return 0;

    if (GetLabel(line, command) == true)
        labelTable[command] = memoryPos;
    if (GetCommand(line, command) != 0)
        return 0;

    if (opcodes.find(command) ==  opcodes.end())
    {
        std::cout << "Line " << std::to_string(lineCount) << std::endl;
        std::cout << "ERROR: Invalid command!" << std::endl;
        return 2;
    }

    mode = GetMode_Label(command);
    if (EvalueLabel(line, memoryPos, mode != AddressMode::Relative) == 0)
        ;
    else if (EvalueParam(line, value, mode) != 0)
    {
        std::cout << "Line " << std::to_string(lineCount) << std::endl;
        std::cout << "ERROR: Invalid argumuments!" << std::endl;;
        return 2;
    };

    if (ApplyMode(command, mode) != 0)
    {
        std::cout << "Line " << std::to_string(lineCount) << std::endl;
        std::cout << "ERROR: Invalid format for the command \"" << command << "\"" << std::endl;
        return 2;
    }

    Byte command_opcode = opcodes[command];
    buffer.push_back(command_opcode);
    memoryPos++, lineCount++;
    
    if (mode == AddressMode::Default)
        return 0;

    if (value > 255 || ModeUsesWord(mode) == true)
    { // Write word
        buffer.push_back(value & 0x00FF);
        buffer.push_back((value & 0xFF00) >> 8);
        memoryPos += 2;
    }
    else
    { // Write byte
        buffer.push_back(value);
        memoryPos++;
    }

    return 0;
}

bool Assembler::ModeUsesWord(AddressMode mode)
{
    return (mode == AddressMode::Absolute || mode == AddressMode::AbsoluteX || mode == AddressMode::AbsoluteY || mode == AddressMode::Label);
}
uMap_ModeToRegex Assembler::LoadAddressRegex()
{
    return uMap_ModeToRegex{
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
        {AddressMode::Default, std::regex("[A-Za-z]{3} *$")}};
}
AddressMode Assembler::GetMode_Label(string instruction)
{
    if (instruction == "BPL" || instruction == "BMI" || instruction == "BVC" || instruction == "BVS" || instruction == "BCC" || instruction == "BCS" || instruction == "BNE" || instruction == "BEQ")
        return AddressMode::Relative;
    else
        return AddressMode::Label;
}

Word Assembler::ConvertFromBase(string value, Byte base)
{
    Word output = 0;

    for (size_t i = 0; i < value.length(); i++)
    {
        // Handling bases with letters
        if (value[i] >= 'a' && value[i] <= 'f')
            output = (output * base) + (value[i] - 87);
        else if (value[i] >= 'A' && value[i] <= 'F')
            output = (output * base) + (value[i] - 55);
        // All bases from 10 below
        else if (isdigit(value[i]))
            output = (output * base) + (value[i] - '0');
        else
            break;
    }

    return output;
}

bool Assembler::RemoveComment(std::string &line)
{
    line = line.substr(0, line.find(';'));
    return line.empty();
}

uint8_t Assembler::GetCommand(const std::string &line, std::string &command)
{
    if (line.empty())
        return 1;

    std::regex command_pattern(INSTRUCTION_PATTERN);
    std::smatch result;

    std::regex_search(line, result, command_pattern);
    if (result[0].matched == false)
        return 2;

    command = result[1].str();
    return 0;
}
uint8_t Assembler::ApplyMode(std::string &command, AddressMode mode)
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
    case AddressMode::Default:
    case AddressMode::Relative:
    case AddressMode::Label:
        break;
    default:
        return 1;
    }

    if (opcodes.exists(command) == false)
        return 2;
    else
        return 0;
}

uint8_t Assembler::EvalueParam(const std::string &line, Word &value, AddressMode &mode)
{
    std::smatch result;
    for (auto const &pattern : modeTable)
    {

        std::regex_search(line, result, pattern.second);

        // Result 0 return the entire match
        // Result 1 return the value and, if included, the base operand
        // Result 2 return the value only
        if (result[0].matched)
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

bool Assembler::GetLabel(const std::string &line, std::string &label)
{
    if (line.empty())
        return false;

    std::regex label_pattern(LABEL_PATTERN);
    std::smatch result;
    std::regex_search(line, result, label_pattern);

    if (result[0].matched == false)
        return false;

    label = result[1].str();
    return true;
}
uint8_t Assembler::EvalueLabel(const string &line, Word memoryPos, bool isWord)
{
    std::smatch result;
    std::regex_search(line, result, std::regex(INSTRUCTION_PATTERN " *" LABEL_REF_PATTERN));

    if (result[0].matched)
    {
        labelReferences[memoryPos + 1] = pair_LabelMode(result[3].str(), isWord);
        return 0;
    }
    return 1;
}

uint8_t Assembler::ResolveLabels()
{
    for (auto &&position : labelReferences)
    {
        if (labelTable.find(position.second.first) == labelTable.end())
        {
            std::cout << "ERROR: Non-declared label found!" << std::endl;
            return 1;
        }

        Word replacePos = labelTable[position.second.first];

        if (position.second.second == true)
        {   // Word value
            buffer[position.first] = (Byte) replacePos & 0x00FF;
            buffer[position.first+1] = (Byte) ((replacePos & 0xFF00) >> 8);
        }
        else
        {   // Byte value
            replacePos -= position.first + 1;
            buffer[position.first] = (Byte) replacePos;
        }
    }

    return 0;
}