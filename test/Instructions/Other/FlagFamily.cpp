#include "../CPU_Model.h"

class Flag_Test : public M6502 {};

TEST_F(Flag_Test, CLC_Test)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Carry = 1;
    memory[0xFFFC] = Instruction::CLC;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_FALSE(processor.Flags.Break);
}

TEST_F(Flag_Test, SEC_Test)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Carry = 0;
    memory[0xFFFC] = Instruction::SEC;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_TRUE(processor.Flags.Break);
}
TEST_F(Flag_Test, CLD_Test)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Carry = 1;
    memory[0xFFFC] = Instruction::CLD;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_FALSE(processor.Flags.Decimal);
}

TEST_F(Flag_Test, SED_Test)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Carry = 0;
    memory[0xFFFC] = Instruction::SEC;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_TRUE(processor.Flags.Decimal);
}
TEST_F(Flag_Test, CLI_Test)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Carry = 1;
    memory[0xFFFC] = Instruction::CLI;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_FALSE(processor.Flags.Interupt);
}

TEST_F(Flag_Test, SEI_Test)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Carry = 0;
    memory[0xFFFC] = Instruction::SEC;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_TRUE(processor.Flags.Interupt);
}

TEST_F(Flag_Test, CLV_Test)
{
    // Given
    const uint32_t CYCLES_1 = 2;
    const uint32_t CYCLES_2 = 2;
    
    processor.RegA = 0xA0;
    memory[0xFFFC] = Instruction::ADC_IM;
    memory[0xFFFD] = 0xA0;
    memory[0xFFFE] = Instruction::CLV;

    // First
    uint32_t cycles_executed = processor.Execute(CYCLES_1, memory);
    EXPECT_EQ(cycles_executed, CYCLES_1);
    EXPECT_TRUE(processor.Flags.OverFlow);

    // Then
    cycles_executed = processor.Execute(CYCLES_2, memory);
    EXPECT_EQ(cycles_executed, CYCLES_1);
    EXPECT_FALSE(processor.Flags.OverFlow);
}