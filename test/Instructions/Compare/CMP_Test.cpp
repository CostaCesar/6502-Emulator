#include "../CPU_Model.h"

class CMP_Test : public INS_6502 {};

TEST_F(CMP_Test, CMP_Immediate)
{
    // Given
    const uint32_t CYCLES = 2;
    const Byte VALUE_1 = 0xAA;
    const Byte VALUE_2 = 0xAA;

    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::CMP_IM;
    memory[0xFFFD] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[0xFFFD], VALUE_2);

    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CMP_Test, CMP_ZeroPage)
{
    // Given
    const uint32_t CYCLES = 3;
    const Byte VALUE_1 = 0x12;
    const Byte VALUE_2 = 0x10;
    const Byte POSITION = 0x10;

    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::CMP_ZP;
    memory[0xFFFD] = POSITION;
    memory[POSITION] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[POSITION], VALUE_2);

    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CMP_Test, CMP_ZeroPage_OffsetX)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE_1 = 0xCC;
    const Byte VALUE_2 = 0xDD;
    const Byte POSITION = 0x10;
    const Byte OFFSET = 0x10;

    processor.RegA = VALUE_1;
    processor.RegX = OFFSET;

    memory[0xFFFC] = Set_6502::CMP_ZPX;
    memory[0xFFFD] = POSITION;
    memory[POSITION + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[POSITION + OFFSET], VALUE_2);

    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CMP_Test, CMP_ZeroPage_OffsetX_Wrapping)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE_1 = 0xCC;
    const Byte VALUE_2 = 0xDD;
    const Byte POSITION = 0x90;
    const Byte OFFSET = 0xA0;

    processor.RegX = OFFSET;
    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::CMP_ZPX;
    memory[0xFFFD] = POSITION;
    memory[(Byte) (POSITION + OFFSET)] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[(Byte) (POSITION + OFFSET)], VALUE_2);

    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CMP_Test, CMP_Absolute)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE_1 = 0xE0;
    const Byte VALUE_2 = 0xDD;
    const Word POSITION = 0x9876;

    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::CMP_AB;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[POSITION], VALUE_2);

    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CMP_Test, CMP_Absolute_OffsetX)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE_1 = 0xFF;
    const Byte VALUE_2 = 0xDD;
    const Word POSITION = 0x4810;
    const Byte OFFSET = 0xA0;

    processor.RegX = OFFSET;
    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::CMP_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[POSITION + OFFSET], VALUE_2);

    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CMP_Test, CMP_Absolute_OffsetX_CrossPage)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte VALUE_1 = 0xEE;
    const Byte VALUE_2 = 0xDD;
    const Word POSITION = 0xECA6;
    const Byte OFFSET = 0xDE;

    processor.RegX = OFFSET;
    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::CMP_ABX;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[POSITION + OFFSET], VALUE_2);

    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CMP_Test, CMP_Absolute_OffsetY)
{
    // Given
    const uint32_t CYCLES = 4;
    const Byte VALUE_1 = 0x80;
    const Byte VALUE_2 = 0x80;
    const Word POSITION = 0x4810;
    const Byte OFFSET = 0xA0;

    processor.RegY = OFFSET;
    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::CMP_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[POSITION + OFFSET], VALUE_2);

    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CMP_Test, CMP_Absolute_OffsetY_CrossPage)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte VALUE_1 = 0x80;
    const Byte VALUE_2 = 0x80;
    const Word POSITION = 0xECA6;
    const Byte OFFSET = 0xDE;

    processor.RegY = OFFSET;
    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::CMP_ABY;
    memory.WriteWord(0xFFFD, POSITION);
    memory[POSITION + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[POSITION + OFFSET], VALUE_2);

    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CMP_Test, CMP_Indirect_OffsetX)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte VALUE_1 = 0xFE;
    const Byte VALUE_2 = 0xFF;
    const Byte POSITION_1 = 0x80;
    const Word POSITION_2 = 0xECA6;
    const Byte OFFSET = 0xDE;

    processor.RegX = OFFSET;
    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::CMP_IDX;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord((Byte) (POSITION_1 + OFFSET), POSITION_2);
    memory[POSITION_2] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[POSITION_2], VALUE_2);

    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CMP_Test, CMP_Indirect_OffsetY)
{
    // Given
    const uint32_t CYCLES = 5;
    const Byte VALUE_1 = 0x00;
    const Byte VALUE_2 = 0xFF;
    const Byte POSITION_1 = 0x80;
    const Word POSITION_2 = 0x1060;
    const Byte OFFSET = 0x12;

    processor.RegY = OFFSET;
    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::CMP_IDY;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord(POSITION_1, POSITION_2);
    memory[POSITION_2 + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[POSITION_2 + OFFSET], VALUE_2);

    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_FALSE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_FALSE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}
TEST_F(CMP_Test, CMP_Indirect_OffsetY_CrossPage)
{
    // Given
    const uint32_t CYCLES = 6;
    const Byte VALUE_1 = 0xEF;
    const Byte VALUE_2 = 0x00;
    const Byte POSITION_1 = 0x80;
    const Word POSITION_2 = 0xECCD;
    const Byte OFFSET = 0xFC;

    processor.RegY = OFFSET;
    processor.RegA = VALUE_1;

    memory[0xFFFC] = Set_6502::CMP_IDY;
    memory[0xFFFD] = POSITION_1;
    memory.WriteWord(POSITION_1, POSITION_2);
    memory[POSITION_2 + OFFSET] = VALUE_2;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE_1);
    EXPECT_EQ(memory[POSITION_2 + OFFSET], VALUE_2);

    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.OverFlow);
    EXPECT_TRUE(processor.Flags.Negative);
    EXPECT_FALSE(processor.Flags.Break);
    EXPECT_TRUE(processor.Flags.Carry);
    EXPECT_FALSE(processor.Flags.Interupt);
    EXPECT_FALSE(processor.Flags.Decimal);
}