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

// Beware of multiple matches!
uMap_ModeToRegex LoadAddressRegex();

Word ConvertFromBase(string value, Byte base);
bool ModeUsesWord(AddressMode mode);
AddressMode GetMode_Label(string instruction);

uint8_t OpenFile_In(std::ifstream &inFile, const char *inFilePath);
uint8_t OpenFile_Out(std::ofstream& outFile, const char *outFilePath);
void CloseFiles(std::ifstream &inFile, std::ofstream &outFile);
void DeleteFile(const char *filePath);

uint8_t GetArgs(int argc, char** argv, std::string &outFilePath);

bool RemoveComment(std::string &line);
uint8_t GetCommand(const string &line, string &command);
bool GetLabel(const std::string &line, std::string &label);

uint8_t EvalueLabel(const string &line, Word memoryPos,
    uMap_PosToLabel &labelReferences, bool isWord);
uint8_t EvalueParam(const string &line, Word &value, AddressMode &mode,
    const uMap_ModeToRegex &addressTable);
uint8_t ApplyMode(string &command, AddressMode mode, InstructionSet& opcodes);

uint8_t ReplaceLabels(const uMap_PosToLabel &locations, uMap_LabelToPos &labels,
    std::ofstream &binaryData);

#endif