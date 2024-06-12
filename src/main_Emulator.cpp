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

#define HIDE_INPUT std::cout << "                              " << std::endl
#define CONFIRM_ACTION std::cout << "Press ENTER to continue" << std::endl; std::getchar();
struct EmulatorStatus
{
    bool autoRun = false; 
    bool quit = false; 
    bool running = false;
    bool viewing = false;

    uint32_t clock = 1000;

    std::mutex mutex_emulator;
    std::condition_variable signal;
} G_state;

void Screen_CPU(CPU &Processor, const Memory &Ram)
{
    RESET_CURSOR(0, 1);

    printf("%c Stack Pointer: %#02x \n", (G_state.autoRun ? '*' : ' '), Processor.StackPointer);
    printf("Address Pointer: %#04x \n", Processor.ProgramCounter);
    printf("Flags:   (Negativ=%d) (Overflow=%d) (Break=%d) \n",
           Processor.Flags.Negative, Processor.Flags.OverFlow, Processor.Flags.Break);
    printf("(Zero=%d) (Decimal=%d) (Interupt=%d) (Carry=%d) \n",
           Processor.Flags.Zero, Processor.Flags.Decimal, Processor.Flags.Interupt, Processor.Flags.Carry);
    printf("Current Instruction: %s \n", Processor.Instructions[Ram[Processor.ProgramCounter]].c_str());
    printf("==================================================\n");
    printf("[V] View Memory [S] View Stack [Z] View Zero Page \n");
    printf("[ENTER] Execute [Q] Exit       [X] Toggle Autorun \n");
    printf("==================================================\n");
    printf("[User@Emulator]$ ");

    std::flush(std::cout);
    return;
}
void Screen_Memory(const Memory& Ram, Word StartPos = 0, Word EndPos = 0xFFFF)
{
    CLEAR_SCREEN;
    RESET_CURSOR(0, 0);
    
    if(EndPos < StartPos)
    {
        std::cout << "END OF MEMORY" << std::endl;
        return;
    }

    for(Word i = StartPos; i < EndPos; i+=0x10)
    {
        printf("0x%04x", i);
        for(Byte j = 0; j < 0x10; j++)
        {
            printf(" %02x", Ram[i+j]);
        }
        printf("\n");
    }
    std::flush(std::cout);

    return;
}
void Paged_Screen_Memory(const Memory& Ram)
{
    static Word page = 0;
    bool validInput = true;

    while(true)
    {
        if(validInput)
            Screen_Memory(Ram, page*0x100, (page + 1)*0x100);
        else RESET_CURSOR(0, 16);
        validInput = true;
        
        printf("===================================\n");
        printf("[A] Previous           [D] Next    \n");
        printf("[S <address>] Go to    [Q] Exit    \n");
        printf("===================================\n");
        printf("[User@Emulator]$ ");
        std::flush(std::cout);

        uint8_t userInput = std::getchar();
        RESET_CURSOR(17, 20);
        HIDE_INPUT;
        
        // Remove leftorver '\n'
        if(userInput != 13 && userInput != 10 && userInput != 's' && userInput != 'S')
            std::getchar();

        switch (userInput)
        {
        case 'a':
        case 'A':
            page--;
            break;
        case 'd':
        case 'D':
            page++;
            break;
        case 's':
        case 'S':
            // Redo S (in case of early ENTER)
            RESET_CURSOR(17, 20);
            std::cout << (char)userInput << ' ';
            std::cout.flush();

            std::cin >> std::hex >> page >> std::dec;
            // Wrap as if it was Byte
            page = (page / 0xFF) % 0x100;
            // Consume leftover '\n'
            std::getchar();
            break;
        case 'q':
        case 'Q':
            CLEAR_SCREEN;
            return;
        default:
            validInput = false;
            continue;
        }
    }
    return;
}

void Thread_CPU(CPU &Processor, Memory &Ram)
{
    while (!G_state.quit)
    {
        Screen_CPU(Processor, Ram);
        
        std::unique_lock<std::mutex> lock(G_state.mutex_emulator);
        if(!G_state.autoRun) 
        {   // Step Mode 
            G_state.signal.wait(lock, []{ return G_state.running; });
        }
        else 
        {   // Autorun Mode
            std::this_thread::sleep_for(std::chrono::milliseconds(G_state.clock));
            // If viewing others screens (memory, etc), wait for it to close
            G_state.signal.wait(lock, []{ return !G_state.viewing; }); 
        }
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

    CLEAR_SCREEN;
    Variant Chip = CHIP_STANDART;
    if(argc < 3) std::cout << "WARNING: No chip variant specified! Assuming base chip [6502]" << std::endl;
    else Chip = (Variant) (argv[2][0] - '0');

    Memory Ram;
    Word Address = 0x1000;

    Ram.Initialise();
    CPU Processor(Chip, Address);

    for(Address; inFile.peek() != EOF; Address++)
    {
        inFile.read((char *) &Ram[Address], sizeof(Byte));
    }
    inFile.close();

    std::thread emulation(Thread_CPU, std::ref(Processor), std::ref(Ram));
    
    uint8_t userInput = 0;
    auto OnScreenExit = [&Processor, &Ram]
    {
        CLEAR_SCREEN;
        Screen_CPU(Processor, Ram);
        // Pause for autorun Mode
        if(G_state.autoRun)
            std::this_thread::sleep_for(std::chrono::milliseconds(G_state.clock));
        // Resume CPU thread
        G_state.viewing = false;
        G_state.signal.notify_all();
    };

    while (true)
    {
        if(G_state.quit)
        {
            emulation.join();
            CLEAR_SCREEN;
            return 0;
        }

        userInput = std::getchar();
        RESET_CURSOR(17, 10);
        HIDE_INPUT;

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
        case 'z':
        case 'Z':
            G_state.viewing = true;
            Screen_Memory(Ram, 0, 0xFF);
            CONFIRM_ACTION;
            OnScreenExit();
            continue;
        case 's':
        case 'S':
            G_state.viewing = true;
            Screen_Memory(Ram, 0x100, 0x1FF);
            CONFIRM_ACTION;
            OnScreenExit();
            continue;
        case 'v':
        case 'V':
            G_state.viewing = true;
            Paged_Screen_Memory(Ram);
            OnScreenExit();
            continue;
        case 'q':
        case 'Q':
            G_state.quit = true;
            break;
        default:
            continue;
        }
        
        // Singal emulator to execute command (Step Mode)
        std::lock_guard<std::mutex> lock(G_state.mutex_emulator);
        G_state.running = true;
        G_state.signal.notify_all();
    }

    emulation.join(); 
    return 1;
}
