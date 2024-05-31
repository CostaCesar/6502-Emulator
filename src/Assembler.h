#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <regex>
#include "Base.h"
#include "Instructions.h"

using std::string;

namespace AssemblerDefs
{
    enum class BaseNumber : Byte
    {
        HexaDec = 16,
        Decimal = 10,
        Octal = 8,
        Binary = 2
    };

    enum class AddressMode : Byte
    {
        Immediate = 0,
        ZeroPage = 1,
        ZeroPageX = 2,
        ZeroPageY = 3,
        Absolute = 4,
        AbsoluteX = 5,
        AbsoluteY = 6,
        IndirectX = 7,
        IndirectY = 8,
        Acumulator = 9,
        Default = 10,
        Relative = 11,
        Label = 12
    };

    typedef std::unordered_map<AddressMode, std::regex> uMap_ModeToRegex;
    typedef std::pair<std::string, bool> pair_LabelMode;
    typedef std::unordered_map<Word, pair_LabelMode> uMap_PosToLabel;
    typedef std::unordered_map<std::string, Word> uMap_LabelToPos;
}

using namespace AssemblerDefs;
class Assembler
{
public:
    Assembler();
    Assembler (const Assembler&) = delete;
    Assembler& operator= (const Assembler&) = delete;

    uint8_t CompileLine(string currentLine);
    uint8_t ResolveLabels();
    uint8_t WriteBuffer(std::ofstream &outFile);

private:
    std::vector<Byte> buffer;
    InstructionSet opcodes;

    Word memoryPos;
    uint64_t lineCount;

    uMap_ModeToRegex modeTable;
    uMap_LabelToPos labelTable;
    uMap_PosToLabel labelReferences;

    // Beware of multiple matches!
    uMap_ModeToRegex LoadAddressRegex();

    Word ConvertFromBase(string value, Byte base);
    bool ModeUsesWord(AddressMode mode);
    bool RemoveComment(std::string &line);

    uint8_t GetCommand(const string &line, string &command);
    uint8_t EvalueParam(const string &line, Word &value, AddressMode &mode);

    bool GetLabel(const std::string &line, std::string &label);
    uint8_t EvalueLabel(const string &line, Word memoryPos, bool isWord);

    uint8_t ApplyMode(string &command, AddressMode mode);
    AddressMode GetMode_Label(string instruction);
};
#endif