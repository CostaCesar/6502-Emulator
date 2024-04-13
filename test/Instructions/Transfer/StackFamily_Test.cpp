#include "../CPU_Model.h"

class Stack_Test : public INS_6502 {};

TEST_F(Stack_Test, TSX_Basic)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = 0x01;

    processor.Reset();
    processor.RegX = 0x00;
    processor.StackPointer = VALUE;
    processor.Flags.Zero = processor.Flags.Negative = true;

    memory[0xFFFC] = Set_6502::TSX;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
};

TEST_F(Stack_Test, TSX_ZeroFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = 0x00;

    processor.Reset();
    processor.RegX = 0x00;
    processor.StackPointer = VALUE;
    processor.Flags.Zero = processor.Flags.Negative = false;
    
    memory[0xFFFC] = Set_6502::TSX;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
};

TEST_F(Stack_Test, TSX_NegativeFlag)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = 0xF1;

    processor.Reset();
    processor.RegX = 0x00;
    processor.StackPointer = VALUE;
    processor.Flags.Zero = processor.Flags.Negative = false;
    
    memory[0xFFFC] = Set_6502::TSX;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegX, VALUE);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
};

TEST_F(Stack_Test, TXS)
{
    // Given
    const uint32_t CYCLES = 2;
    const uint16_t VALUE = 0xF1;

    processor.Reset();
    processor.RegX = VALUE;

    memory[0xFFFC] = Set_6502::TXS;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.StackPointer, VALUE);
    FlagsExcept(processor);
};

TEST_F(Stack_Test, PHA)
{
    // Given
    const uint32_t CYCLES = 3;
    const uint8_t VALUE = 0x64;

    processor.Reset();
    processor.RegA = VALUE;

    memory[0xFFFC] = Set_6502::PHA;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[processor.Stack_AsWord()+1], processor.RegA);
    EXPECT_FALSE(processor.FlagStatus);
};

TEST_F(Stack_Test, PHP) // Oh no, PHP is invading the system!
{
    // Given
    const uint32_t CYCLES = 3;
    const uint8_t VALUE = 0b10101010;

    processor.Reset();
    processor.FlagStatus = VALUE;

    memory[0xFFFC] = Set_6502::PHP;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(memory[processor.Stack_AsWord()+1], processor.FlagStatus);
};

TEST_F(Stack_Test, PLA)
{
    // Given
    const uint32_t CYCLES = 4;
    const uint8_t VALUE = 0x64;

    processor.Reset();

    memory[processor.Stack_AsWord()] = VALUE;
    memory[0xFFFC] = Set_6502::PLA;
    processor.StackPointer--;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    FlagsExcept_NegvZero(processor);
};

TEST_F(Stack_Test, PLA_ZeroFlag)
{
    // Given
    const uint32_t CYCLES = 4;
    const uint8_t VALUE = 0x00;

    processor.Reset();
    
    memory[processor.Stack_AsWord()] = VALUE;
    memory[0xFFFC] = Set_6502::PLA;
    processor.StackPointer--;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_TRUE(processor.Flags.Zero);
    EXPECT_FALSE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
};

TEST_F(Stack_Test, PLA_NegativeFlag)
{
    // Given
    const uint32_t CYCLES = 4;
    const uint8_t VALUE = 0xF1;

    processor.Reset();

    memory[processor.Stack_AsWord()] = VALUE;
    memory[0xFFFC] = Set_6502::PLA;
    processor.StackPointer--;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.RegA, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
    EXPECT_FALSE(processor.Flags.Zero);
    EXPECT_TRUE(processor.Flags.Negative);
    FlagsExcept_NegvZero(processor);
};


TEST_F(Stack_Test, PLP)
{
    // Given
    const uint32_t CYCLES = 4;
    const uint8_t VALUE = 0b10101010;

    processor.Reset();

    memory[processor.Stack_AsWord()] = VALUE;
    memory[0xFFFC] = Set_6502::PLP;
    processor.StackPointer--;

    // When
    uint32_t cycles_executed = processor.Execute(CYCLES, memory);

    // Execute
    EXPECT_EQ(cycles_executed, CYCLES);
    EXPECT_EQ(processor.FlagStatus, VALUE);
    EXPECT_EQ(processor.StackPointer, 0x00FF);
};




