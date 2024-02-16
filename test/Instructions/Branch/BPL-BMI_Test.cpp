#include "../CPU_Model.h"

class BPL_BMI_Test : public M6502 {};

TEST_F(BPL_BMI_Test, BPL_Pass)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte OFFSET = 0x10;
    const Word BASE_POS = 0x1000;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Negative = 1;

    memory[BASE_POS - 1] = Instruction::BPL;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + 1);
    EXPECT_TRUE(processor.Flags.Negative);
}
TEST_F(BPL_BMI_Test, BPL_Branch)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte OFFSET = 0x6;
    const Word BASE_POS = 0xE004;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Negative = 0;

    memory[BASE_POS - 1] = Instruction::BPL;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + (int8_t) OFFSET);
    EXPECT_FALSE(processor.Flags.Negative);
}
TEST_F(BPL_BMI_Test, BPL_PageCross)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = -12;
    const Word BASE_POS = 0xE009;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Negative = 0;

    memory[BASE_POS - 1] = Instruction::BPL;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + (int8_t) OFFSET + 1);
    EXPECT_FALSE(processor.Flags.Negative);
}
TEST_F(BPL_BMI_Test, BMI_Pass)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte OFFSET = 0x10;
    const Word BASE_POS = 0x1000;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Negative = 0;

    memory[BASE_POS - 1] = Instruction::BMI;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + 1);
    EXPECT_FALSE(processor.Flags.Negative);
}
TEST_F(BPL_BMI_Test, BMI_Branch)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte OFFSET = 0x80;
    const Word BASE_POS = 0xE004;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Negative = 1;

    memory[BASE_POS - 1] = Instruction::BMI;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + (int8_t) OFFSET + 1);
    EXPECT_TRUE(processor.Flags.Negative);
}
TEST_F(BPL_BMI_Test, BMI_PageCross)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte OFFSET = 0x64;
    const Word BASE_POS = 0xE009;
    
    processor.Reset(BASE_POS - 1);
    processor.Flags.Negative = 1;

    memory[BASE_POS - 1] = Instruction::BMI;
    memory[BASE_POS] = OFFSET;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.ProgramCounter, BASE_POS + (int8_t) OFFSET);
    EXPECT_TRUE(processor.Flags.Negative);
}
