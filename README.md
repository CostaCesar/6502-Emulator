# 6502-Emulator

![Static Badge](https://img.shields.io/badge/Status-Prototyping-yellow)
![Static Badge](https://img.shields.io/badge/Made_with-C%2B%2B-blue)
![Static Badge](https://img.shields.io/badge/Tests-GoogleTest-orange)
![Static Badge](https://img.shields.io/badge/Build-CMake-red)
![Static Badge](https://img.shields.io/badge/Platform-Universal-white)

## About / Sobre
### English
This is a emulation project of the 8-bit [MOS Tecnology 6502](https://en.wikipedia.org/wiki/6502), used one way or another by many sytems from the 70's and 80's, such as the
[NES](https://en.wikipedia.org/wiki/Nintendo_Entertainment_System), the [Commodore 64](https://en.wikipedia.org/wiki/Commodore_64) and the [Apple II](https://en.wikipedia.org/wiki/Apple_II).
It was (totally) inspired by [Dave Poo](https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37) series, where he implements step by step the processor features.
So, I decided to expand the scope, by adding an interface to the emulator, allowing users to run Assembly programs and see the result and the inner status of the system (CPU, Memory, I/O, etc),
while also creating a starting point to a full-scale emulation of a 6502-based computer, by me or by anyone who wants it.
<br/>
<br/>
### Português
Esse é um projeto de emulação do processador 8-bits [MOS Tecnology 6502](https://pt.wikipedia.org/wiki/6502), utilizado de uma forma ou outra em vários sistemas durante os anos 70 e 80,
como o [NES](https://pt.wikipedia.org/wiki/Nintendo_Entertainment_System), o [Commodore 64](https://pt.wikipedia.org/wiki/Commodore_64) e o [Apple II](https://pt.wikipedia.org/wiki/Apple_II).
Esse projeto é (totalmente) inspirado pela série do youtuber [Dave Poo](https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37), onde ele monta passo a passo o as funcionalidades
do emulador. Decidi então expandir o escopo, adicionando uma interface com o emulador, permitindo o usuário inserir um programa Assembly e ver o seu resultado, junto com o passo a passo dentro do sistema
(CPU, Memória, I/O, etc), e criar uma base para construção de um emulador completo de um computador baseado em 6502, para mim e para quem quiser também.

## Goal / Objetivo
### English
It's planned to have:
- A fast emulator writen in C++
- The 6502 processor, with all of the base instructions
- 64 Kb of accessible memory (no ROM or I/O predefined)
- A program to load Assembly code into the emulator
- An (maybe graphical) interface to visualize the system
- Test suites, to verify the emulation fidelity

### Português
Está planejado para o projeto:
- Um emulador rápido, feito em C++
- O processador 6502, com todas suas intruções base
- 64 Kb de memória total acessível (sem ROM ou I/O predefinidos)
- Um programa para carregar o código Assembly para o emulador
- Uma interface (talvez gráfica) para visualizar o sistema
- Rotinas de teste, para verificar a fidelidade da emulação

## Status / Situação
### English
- Instructions suported:
  - LDA
  - LDX
  - LDY
  - STA
  - STX
  - STY

In summary, the project is in a very early stage, with almost no instructions available and no real interface with the user
<br/>
<br/>
### Português
- Instruções aceitas:
  - LDA
  - LDX
  - LDY
  - STA
  - STX
  - STY

Em resumo, o projeto está em um estágio muito inicial, com quase nenhuma instrução feita e nenhuma interface com o usuário

## Credits / Créditos
###English
Excluding the GoogleTest library (obviously), all of the code in this project was written by me, CostaCesar. That said, as I made clear in the [about](#About-/-Sobre) section, the idea for this
emulator comes from [Dave Poo](https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37) and his original implementation of the 6502. This is just an expansion of it.

I also want to thank [CodingWithMat](https://www.youtube.com/@codingwithmat) for the tutorial in the GoogleTest + CMake usage, as it was of great help for the project.

And if you want to contribute with the project in any way, just contact me in [Github](https://github.com/CostaCesar) or by [Email](mailto:caiocaesarmcosta@gmail.com).

❤️ Any help is deeply appreciated! ❤️
<br/>
<br/>
###Português
Excluindo a biblioteca GoogleTest (é claro), todo o código neste projeto foi feito por mim, CostaCesar. Ainda assim, como deixei claro na sessão [sobre](#About-/-Sobre), a ideia para esse
emulador veio de [Dave Poo](https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37) e sua implementação original do 6502. Esse repositório é apenas uma expansão disso.

Também quero agradecer a [CodingWithMat](https://www.youtube.com/@codingwithmat) pelo tutorial de como usar GoogleTest + Cmake, pois foi de grande ajudar para o projeto.

E se você quer contribuir de alguma forma, pode me chamar no [Github](https://github.com/CostaCesar) ou por [Email](mailto:caiocaesarmcosta@gmail.com).

❤️ Qualquer ajuda é sempre bem vinda! ❤️
