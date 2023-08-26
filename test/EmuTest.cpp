#include <gtest/gtest.h>
#include "../src/Memory.h"
#include "../src/CPU.h"

class M6502_Test : public testing::Test
{
public:
    Memory memory;
    CPU processor;

    virtual void SetUp()
    {
        processor.Reset(memory);
    }

    virtual void TearDown()
    {

    }
};

TEST_F(M6502_Test, BasicProcessorTest)
{
    // start - Hacked code
    memory[0xFFFC] = CPU::INS_JSR;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xAA;
    memory[0xAAAA] = CPU::INS_LDA_IM;
    memory[0xAAAB] = 0xA;
    // end - Hacked code

    processor.Execute(8, memory);
    EXPECT_EQ(processor.RegA, 0xA1);
}

#if 0
int main(int argc, char ** argv)
{
    Memory memory;
    CPU processor;
    processor.Reset(memory);
    
    // start - Hacked code
    memory[0xFFFC] = CPU::INS_JSR;
    memory[0xFFFD] = 0xAA;
    memory[0xFFFE] = 0xAA;
    memory[0xAAAA] = CPU::INS_LDA_IM;
    memory[0xAAAB] = 0xA;
    // end - Hacked code

    processor.Execute(8, memory);
    return 0;
}
#endif