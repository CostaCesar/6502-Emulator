#include "Instructions.h"

Opcode InstructionSet::operator[] (std::string value)
{
    typedef std::map<std::string, Opcode> map;
    map::iterator to_find = Set.find(value);
    
    if(to_find->first == value)
    {
        found = true;
        return to_find->second;
    }
    else
    {
        found = false;
        return Set_6502::NOP;
    }
}
std::string InstructionSet::operator[] (Opcode value)
{
    typedef std::map<std::string, Opcode> map;
    for(map::iterator it = Set.begin(); it != Set.end(); it++)
    {
        if(it->second == value)
        {
            found = true;
            return it->first;
        }
    }
    found = false;
    return std::string("NOP");
}

std::map<std::string, Opcode>::iterator InstructionSet::find(std::string value)
{ 
    typedef std::map<std::string, Opcode> map;
    for(map::iterator it = Set.begin(); it != Set.end(); it++)
    {
        if((value.size() == 3 && it->first.substr(0, 3) == value)
        || it->first == value)
            return Set.begin();
    }
    return Set.end();
}
std::map<std::string, Opcode>::iterator InstructionSet::end()
{ return Set.end();}
