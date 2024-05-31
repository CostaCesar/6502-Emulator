include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY  https://github.com/google/googletest.git
  GIT_TAG         release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

FILE(GLOB_RECURSE Ins_Sources Instructions/*.cpp)
FILE(GLOB_RECURSE Asm_Sources Assembler/*.cpp)

add_executable(Instruction_Test
  ${Ins_Sources}
)
target_link_libraries(Instruction_Test
  PRIVATE
  GTest::GTest
  Instruction
)

add_executable(Assembler_Test
  ${Asm_Sources}
)
target_link_libraries(Assembler_Test
  PRIVATE
  GTest::GTest
  Assembler
)

add_test(InstructionTest
  Instruction_Test
)
add_test(AssemblerTest
  Assembler_Test
)
