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

add_executable(Instruction_Test
  ${Ins_Sources}
)
target_link_libraries(Instruction_Test
  PRIVATE
  GTest::GTest
  Emulator
)

add_test(InstructionTest Instruction_Test)