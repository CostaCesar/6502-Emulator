#include "Assembler.h"

uint8_t OpenFile_In(std::ifstream &inFile, const string &inFilePath)
{
    inFile.open(inFilePath.c_str());
    if (!inFile.is_open())
    {
        std::cout << "ERROR: Could not open file \"" << inFilePath << "\"" << std::endl;
        return 1;
    }

    return 0;
}
uint8_t OpenFile_Out(std::ofstream &outFile, const string &outFilePath)
{
    const char* p_outFilePath = outFilePath.c_str();

    if (std::filesystem::exists(p_outFilePath))
    {
        std::cout << "WARNING: A file with the name \"" << p_outFilePath << "\" already exists! Overwrite [y/N]? " << std::endl;

        char command = getchar();
        if (command == 'n' || command == 'N' || command == '\n')
            return 1;
    }
    outFile.open(p_outFilePath, std::ios::binary);
    if (!outFile.is_open())
    {
        std::cout << "ERROR: Could not create file \"" << p_outFilePath << "\"" << std::endl;
        return 1;
    }

    return 0;
}
void CloseFiles(std::ifstream &inFile, std::ofstream &outFile)
{
    if (inFile.is_open())
        inFile.close();
    if (outFile.is_open())
        outFile.close();
    return;
}
void DeleteFile(const char *filePath)
{
    if (std::filesystem::exists(filePath))
        std::remove(filePath);
    return;
}
uint8_t GetArgs(int argc, char **argv, std::string &inFilePath, std::string &outFilePath)
{
    if (argc < 2)
    {
        std::cout << "ERROR: No input file specified!" << std::endl;
        return 1;
    }

    if (argc == 3) outFilePath = argv[2];
    else outFilePath = "a.bin";
    
    inFilePath = argv[1];
    return 0;
}
void Error(std::ifstream &inFile, std::ofstream &outFile, const string& outPath)
{
    CloseFiles(inFile, outFile);
    DeleteFile(outPath.c_str());
}

int main(int argc, char ** argv)
{
    string inPath, outPath;

    if(GetArgs(argc, argv, inPath, outPath) != 0) return 1;

    std::ifstream inFile;
    std::ofstream outFile;

    if(OpenFile_In(inFile, inPath) != 0 || OpenFile_Out(outFile, outPath))
    {
        CloseFiles(inFile, outFile);
        return 1;
    }

    Assembler program;
    string line;

    while(getline(inFile, line))
    {
        if(program.CompileLine(line) != 0)
        {
            Error(inFile, outFile, outPath);
            return 1;
        }
    }   
    if(program.ResolveLabels() != 0)
    {
        Error(inFile, outFile, outPath);
        return 2;
    }
    if(program.WriteBuffer(outFile) != 0)
    {
        Error(inFile, outFile, outPath);
        return 3;
    }

    CloseFiles(inFile, outFile);
    return 0;
}