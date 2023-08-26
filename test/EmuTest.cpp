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

TEST_F(M6502_Test, LDA_Imediatate)
{
    // Given
    memory[0xFFFC] = CPU::INS_LDA_IM;
    memory[0xFFFD] = 0xA;

    // When
    processor.Execute(2, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}

TEST_F(M6502_Test, LDA_ZeroPage)
{
    // Given
    memory[0xFFFC] = CPU::INS_LDA_ZP;
    memory[0xFFFD] = 0x0010;
    memory[0x0010] = 0xA;

    // When
    processor.Execute(3, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}

TEST_F(M6502_Test, LDA_ZeroPage_OffsetX)
{
    // Given
    processor.RegX = 0x5;
    memory[0xFFFC] = CPU::INS_LDA_ZPX;
    memory[0xFFFD] = 0x0010;
    memory[0x0015] = 0xA;

    // When
    processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}

TEST_F(M6502_Test, LDA_ZeroPage_OffsetX_Wrapping)
{
    // Given
    processor.RegX = 0xFF;
    memory[0xFFFC] = CPU::INS_LDA_ZPX;
    memory[0xFFFD] = 0x0080;
    memory[0x007F] = 0xA;

    // When
    processor.Execute(4, memory);

    // Execute
    EXPECT_EQ(processor.RegA, 0xA);
    EXPECT_FALSE(processor.F_Zero);
    EXPECT_FALSE(processor.F_Negative);
}


// INS_LDA_IM = 0xA9,  // 2 cycles: Load To RegA Imediate value
// INS_LDA_ZP = 0xA5,  // 3 cyles: Load to RegA value from memory
// INS_LDA_ZPX = 0xB5, // 4 cyles: Load to RegA value from memory + offset from RegX
// INS_JSR = 0x20;     // 6 cyles: Branch to subroutine