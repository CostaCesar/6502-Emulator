include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY  https://github.com/google/googletest.git
  GIT_TAG         release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(INS_Test
  LDA_Test.cpp
  LDX_Test.cpp
  LDY_Test.cpp
  STA_Test.cpp
  STX_Test.cpp
  STY_Test.cpp
)
target_link_libraries(INS_Test
  PRIVATE
  GTest::GTest
  Emulator
)

add_test(InstructionTest INS_Test)