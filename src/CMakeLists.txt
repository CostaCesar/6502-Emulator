add_library(Instruction
    Memory.cpp
    CPU_Commons.cpp
    CPU_Execution.cpp
    Instructions.cpp
)

add_library(Assembler
    Instructions.cpp
    Assembler.cpp
)

add_executable(Assembler_Program
    main_Assembler.cpp
)
target_link_libraries(Assembler_Program
  Assembler
)
add_executable(Emulator_Program
    main_Emulator.cpp
)
target_link_libraries(Emulator_Program
  Instruction
)
set_target_properties(Emulator_Program PROPERTIES
  VS_DEBUGGER_COMMAND_ARGUMENTS "teste.bin")