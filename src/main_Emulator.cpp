#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "Base.h"
#include "CPU.h"
#include "Memory.h"

#ifdef _WIN32
#include <Windows.h>
#define CLEAR_SCREEN system("cls")
#define RESET_CURSOR(x, y) SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), (COORD ){x, y})
#elif __unix
#include <unistd.h>
#define CLEAR_SCREEN system("clear")
#define RESET_CURSOR(x, y) printf("\033[%d;%dH", y, x)
#endif

struct EmulatorStatus
{
    bool autoRun = false; 
    bool quit = false; 
    bool running = false;

    uint32_t clock = 1000;

    std::mutex mutex_emulator;
    std::condition_variable signal;
} G_state;


void Thread_CPU(CPU &Processor, Memory &Ram)
{
    while (!G_state.quit)
    {
        RESET_CURSOR(0, 1);

        printf("%c Stack Pointer: %#02x \n", (G_state.autoRun ? '*' : ' '), Processor.StackPointer);
        printf("Address Pointer: %#04x \n", Processor.ProgramCounter);
        printf("Flags:    (Negativ=%d) (Overflow=%d) (Break=%d) \n",
                Processor.Flags.Negative, Processor.Flags.OverFlow, Processor.Flags.Break);
        printf("(Zero=%d) (Decimal=%d) (Interupt=%d) (Carry=%d) \n",
                Processor.Flags.Zero, Processor.Flags.Decimal, Processor.Flags.Interupt, Processor.Flags.Carry);
        printf("Current Instruction: %s \n", Processor.Instructions[Ram[Processor.ProgramCounter]].c_str());
        printf("==================================================\n");
        printf("[V] View Memory [S] View Stack [Z] View Zero Page \n");
        printf("[ENTER] Execute [Q] Exit       [X] Toggle Autorun \n");
        printf("==================================================\n");
        printf("[User@Emulator]$                                    ");
        
        RESET_CURSOR(17, 10);
        std::flush(std::cout);
        
        std::unique_lock<std::mutex> lock(G_state.mutex_emulator);
        if(!G_state.autoRun) // Wait for I/O Mode 
            { G_state.signal.wait(lock, []{ return G_state.running; }); }
        else // Autorun Mode
            { std::this_thread::sleep_for(std::chrono::milliseconds(G_state.clock)); }
        G_state.running = false;

        Processor.Execute(1, Ram);
    }
    CLEAR_SCREEN;
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

    for(; inFile.peek() != EOF; Address++)
    {
        inFile.read((char *) &Ram[Address], sizeof(Byte));
    }
    inFile.close();
    CLEAR_SCREEN;

    std::thread emulation(Thread_CPU, std::ref(Processor), std::ref(Ram));
    
    uint8_t userInput = 0;
    while (true)
    {
        if(G_state.quit)
        {
            emulation.join();
            CLEAR_SCREEN;
            return 0;
        }

        userInput = std::getchar();
        // Remove leftorver '\n'
        if(userInput != 13 && userInput != 10)
            std::getchar();

        switch (userInput)
        {
        case 13: // ENTER
        case 10: // New-line
            if(G_state.autoRun) continue;
            break;
        case 'x':
        case 'X':
            G_state.autoRun = !G_state.autoRun;
            break;
        case 'q':
        case 'Q':
            G_state.quit = true;
            break;
        default:
            continue;
        }
        
        // Singal emulator to execute command
        std::lock_guard<std::mutex> lock(G_state.mutex_emulator);
        G_state.running = true;
        G_state.signal.notify_all();
    }

    emulation.join(); 
    return 1;
}
