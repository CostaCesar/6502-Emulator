#include "../CPU_Model.h"

class BNE_BEQ_Test : public M6502 {};

TEST_F(BNE_BEQ_Test, BNE_Pass)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte OFFSET = 0x10;
    const Word BASE_POS = 0x1000;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Zero = 1;

    memory[BASE_POS - 1] = Instruction::BNE;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + 1);
    EXPECT_TRUE(processor.Flags.Zero);
}
TEST_F(BNE_BEQ_Test, BNE_Branch)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte OFFSET = 0x6;
    const Word BASE_POS = 0xE004;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Zero = 0;

    memory[BASE_POS - 1] = Instruction::BNE;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + (int8_t) OFFSET);
    EXPECT_FALSE(processor.Flags.Zero);
}
TEST_F(BNE_BEQ_Test, BNE_PageCross)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = -12;
    const Word BASE_POS = 0xE009;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Zero = 0;

    memory[BASE_POS - 1] = Instruction::BNE;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + (int8_t) OFFSET + 1);
    EXPECT_FALSE(processor.Flags.Zero);
}
TEST_F(BNE_BEQ_Test, BEQ_Pass)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte OFFSET = 0x10;
    const Word BASE_POS = 0x1000;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Zero = 0;

    memory[BASE_POS - 1] = Instruction::BEQ;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + 1);
    EXPECT_FALSE(processor.Flags.Zero);
}
TEST_F(BNE_BEQ_Test, BEQ_Branch)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x80;
    const Word BASE_POS = 0xE004;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Zero = 1;

    memory[BASE_POS - 1] = Instruction::BEQ;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + (int8_t) OFFSET + 1);
    EXPECT_TRUE(processor.Flags.Zero);
}
TEST_F(BNE_BEQ_Test, BEQ_PageCross)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte OFFSET = 0x64;
    const Word BASE_POS = 0xE009;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Zero = 1;

    memory[BASE_POS - 1] = Instruction::BEQ;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + (int8_t) OFFSET);
    EXPECT_TRUE(processor.Flags.Zero);
}
