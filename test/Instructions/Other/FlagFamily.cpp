#include "../CPU_Model.h"

class Flag_Test : public INS_6502 {};

TEST_F(Flag_Test, CLC)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Carry = 1;
    memory[0xFFFC] = Set_6502::CLC;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_FALSE(processor.Flags.Carry);
}

TEST_F(Flag_Test, SEC)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Carry = 0;
    memory[0xFFFC] = Set_6502::SEC;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_TRUE(processor.Flags.Carry);
}
TEST_F(Flag_Test, CLD)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Decimal = 1;
    memory[0xFFFC] = Set_6502::CLD;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_FALSE(processor.Flags.Decimal);
}

TEST_F(Flag_Test, SED)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Decimal = 0;
    memory[0xFFFC] = Set_6502::SED;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_TRUE(processor.Flags.Decimal);
}
TEST_F(Flag_Test, CLI)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Carry = 1;
    memory[0xFFFC] = Set_6502::CLI;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_FALSE(processor.Flags.Interupt);
}

TEST_F(Flag_Test, SEI)
{
    // Given
    const uint32_t CYCLES = 2;
    
    processor.Flags.Carry = 0;
    memory[0xFFFC] = Set_6502::SEI;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_TRUE(processor.Flags.Interupt);
}

TEST_F(Flag_Test, CLV)
{
    // Given
    const uint32_t CYCLES_1 = 2;
    const uint32_t CYCLES_2 = 2;
    
    processor.RegA = 0x70;
    memory[0xFFFC] = Set_6502::ADC_IM;
    memory[0xFFFD] = 0x30;
    memory[0xFFFE] = Set_6502::CLV;

    // First
    uint32_t cycles_executed = processor.Execute(CYCLES_1, memory);
    EXPECT_EQ(cycles_executed, CYCLES_1);
    EXPECT_TRUE(processor.Flags.OverFlow);

    // Then
    cycles_executed = processor.Execute(CYCLES_2, memory);
    EXPECT_EQ(cycles_executed, CYCLES_1);
    EXPECT_FALSE(processor.Flags.OverFlow);
}