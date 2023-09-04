# 6502-Emulator

[![pt](https://img.shields.io/badge/Ver_em_portugu%C3%AAs-green)](https://github.com/CostaCesar/6502-Emulator/blob/main/README.pt-br.md)

![Static Badge](https://img.shields.io/badge/Status-Prototyping-yellow)
![Static Badge](https://img.shields.io/badge/Made_with-C%2B%2B-blue)
![Static Badge](https://img.shields.io/badge/Tests-GoogleTest-orange)
![Static Badge](https://img.shields.io/badge/Build-CMake-red)
![Static Badge](https://img.shields.io/badge/Platform-Universal-white)

## About 
This is a emulation project of the 8-bit [MOS Tecnology 6502](https://en.wikipedia.org/wiki/6502), used one way or another by many sytems from the 70's and 80's, such as the
[NES](https://en.wikipedia.org/wiki/Nintendo_Entertainment_System), the [Commodore 64](https://en.wikipedia.org/wiki/Commodore_64) and the [Apple II](https://en.wikipedia.org/wiki/Apple_II).
It was (totally) inspired by [Dave Poo](https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37) series, where he implements step by step the processor features.
So, I decided to expand the scope, by adding an interface to the emulator, allowing users to run Assembly programs and see the result and the inner status of the system (CPU, Memory, I/O, etc),
while also creating a starting point to a full-scale emulation of a 6502-based computer, by me or by anyone who wants it.

## Goal
It's planned to have:
- A fast emulator writen in C++
- The 6502 processor, with all of the base instructions
- 64 Kb of accessible memory (no ROM or I/O predefined)
- A program to load Assembly code into the emulator
- An (maybe graphical) interface to visualize the system
- Test suites, to verify the emulation fidelity

## Status
- Instructions suported:
  - LDA
  - LDX
  - LDY
  - STA
  - STX
  - STY

In summary, the project is in a very early stage, with almost no instructions available and no real interface with the user

## Credits
Excluding the GoogleTest library (obviously), all of the code in this project was written by me, CostaCesar. That said, as I made clear in the [about](#sobre) section, the idea for this
emulator comes from [Dave Poo](https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37) and his original implementation of the 6502. This is just an expansion of it.

I also want to thank [CodingWithMat](https://www.youtube.com/@codingwithmat) for the tutorial in the GoogleTest + CMake usage, as it was of great help for the project.

And if you want to contribute with the project in any way, just contact me in [Github](https://github.com/CostaCesar) or by [Email](mailto:caiocaesarmcosta@gmail.com).

❤️ Any help is deeply appreciated! ❤️

