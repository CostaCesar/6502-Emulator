#include "Instructions.h"

typedef std::unordered_map<std::string, Opcode> map;


bool InstructionSet::exists(std::string value)
{
    return Set.find(value) != Set.end(); 
}
Opcode InstructionSet::operator[] (std::string value)
{
    map::iterator to_find = Set.find(value);
    
    if(to_find->first == value)
        return to_find->second;
    else return Set_6502::NOP;
}
std::string InstructionSet::operator[] (Opcode value)
{
    for(map::iterator it = Set.begin(); it != Set.end(); it++)
    {
        if(it->second == value)
            return it->first;
    }
    return std::string("NOP");
}

std::unordered_map<std::string, Opcode>::iterator InstructionSet::find(std::string value)
{ 
    for(map::iterator it = Set.begin(); it != Set.end(); it++)
    {
        if((value.size() == 3 && it->first.substr(0, 3) == value)
        || it->first == value)
            return Set.begin();
    }
    return Set.end();
}
std::unordered_map<std::string, Opcode>::iterator InstructionSet::end()
{ return Set.end();}
