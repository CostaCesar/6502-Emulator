#include "Assembler_Class.h"
#include "Base.h"

using std::string;

Word ConvertFromBase(string value, Byte base)
{
    Word output = 0;

    for (size_t i = 0 ; i < value.length(); i++)
    {
        // TODO: Wont read Hex because it uses letters
        if(!isdigit(value[i]))
            break;
        output = (output * base) + (value[i] - '0');
    }
    
    return output;
}

void CloseFiles(std::ifstream &inFile, std::ofstream &outFile)
{
    if(inFile.is_open())
        inFile.close();
    if(outFile.is_open())
        outFile.close();
    return;
}