#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include "Base.h"
#include "CPU.h"
#include "Memory.h"

#ifdef _WIN32
#include <Windows.h>
#define RESET_CURSOR() SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), (COORD ){0, 1})
#elif __unix
#include <unistd.h>
#define RESET_CURSOR() printf("\033[0;1H")
#endif

struct EmulatorStatus
{
    bool autoRun = false; 
    bool quit = false; 
    bool stoped = true; 
    uint32_t clock = 1000;
};

void Thread_CPU(CPU &Processor, Memory &Ram, EmulatorStatus& state)
{
    while (!state.quit)
    {
        RESET_CURSOR();
            
        printf("%c Stack Pointer: %#02x \n", (state.autoRun ? '*' : ' '), Processor.StackPointer);
        printf("Address Pointer: %#04x \n", Processor.ProgramCounter);
        printf("Flags:    (Negativ=%d) (Overflow=%d) (Break=%d) \n",
                Processor.Flags.Negative, Processor.Flags.OverFlow, Processor.Flags.Break);
        printf("(Zero=%d) (Decimal=%d) (Interupt=%d) (Carry=%d) \n",
                Processor.Flags.Zero, Processor.Flags.Decimal, Processor.Flags.Interupt, Processor.Flags.Carry);
        printf("Next Instruction: %s", Processor.Instructions[Ram[Processor.ProgramCounter]]);
        printf("==================================================\n");
        printf("[V] View Memory [S] View Stack [Z] View Zero Page \n");
        printf("[ENTER] Execute [Q] Exit       [X] Toggle Autorun \n");
           
        if(!state.autoRun) // Wait for I/O
            while (state.stoped);
        else std::this_thread::sleep_for(std::chrono::milliseconds(state.clock));
        state.stoped = true;

        Processor.Execute(1, Ram);
    }
    return;
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cout << "ERROR: No input file specified!" << std::endl;
        return 1;
    }
    
    std::ifstream inFile(argv[1], std::ios::binary);
    if(!inFile.is_open())
    {
        std::cout << "ERROR: Could not open file \"" << argv[1] << "\"" << std::endl;
        return 1;
    }

    Variant Chip = CHIP_STANDART;
    if(argc < 3) std::cout << "WARNING: No chip variant specified! Assuming base chip [6502]" << std::endl;
    else Chip = (Variant) (argv[2][0] - '0');

    Memory Ram;
    Word Address = 0x1000;

    Ram.Initialise();
    CPU Processor(Chip, Address);
    EmulatorStatus state;

    for(; inFile.peek() != EOF; Address++)
    {
        inFile.read((char *) &Ram[Address], sizeof(Byte));
    }
    inFile.close();

    std::thread emulation(Thread_CPU, std::ref(Processor), std::ref(Ram), std::ref(state));
    
    uint8_t userInput = 0;
    while (true)
    {
        userInput = std::getchar();
        switch (userInput)
        {
        case 13: // ENTER
        case 10: // New-line
            if(state.autoRun) continue;
            break;
        case 'x':
        case 'X':
            state.autoRun = !state.autoRun;
            break;
        case 'q':
        case 'Q':
            state.quit = true;
            emulation.join();
            return 0;
        default:
            break;
        }
        state.stoped = false;
    }

    emulation.join(); 
    return 1;
}
