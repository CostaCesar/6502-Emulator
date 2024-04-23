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
};

// Beware of multiple matches!
std::unordered_map<AddressMode, std::regex> LoadAddressRegex();

Word ConvertFromBase(string value, Byte base);
bool ModeUsesWord(AddressMode mode);

uint8_t OpenFile_In(std::ifstream &inFile, const char *inFilePath);
uint8_t OpenFile_Out(std::ofstream& outFile, const char *outFilePath);
void CloseFiles(std::ifstream &inFile, std::ofstream &outFile);

uint8_t GetArgs(int argc, char** argv, std::string &outFilePath);

void RemoveComment(std::string &line);
uint8_t GetCommand(const std::string &line, std::string &command);
uint8_t EvalueParam(const std::string &line, Word &value, AddressMode &mode,
    const std::unordered_map<AddressMode, std::regex> &addressTable);
uint8_t ApplyMode(std::string &command, AddressMode mode, InstructionSet& opcodes);

#endif