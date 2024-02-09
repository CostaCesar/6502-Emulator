# 6502-Emulator

[![en](https://img.shields.io/badge/See_in_english-red)](https://github.com/CostaCesar/6502-Emulator/blob/main/README.md)

![Static Badge](https://img.shields.io/badge/Estado-Pre%20Alfa-yellow)
![Static Badge](https://img.shields.io/badge/Feito_em-C%2B%2B-blue)
![Static Badge](https://img.shields.io/badge/Testes-GoogleTest-orange)
![Static Badge](https://img.shields.io/badge/Build-CMake-red)
![Static Badge](https://img.shields.io/badge/Platforma-Universal-white)

## Sobre
Esse é um projeto de emulação do processador de 8-bits [MOS Tecnology 6502](https://pt.wikipedia.org/wiki/6502), utilizado de uma forma ou outra em vários sistemas durante os anos 70 e 80,
como o [NES](https://pt.wikipedia.org/wiki/Nintendo_Entertainment_System), o [Commodore 64](https://pt.wikipedia.org/wiki/Commodore_64) e o [Apple II](https://pt.wikipedia.org/wiki/Apple_II).
Esse projeto é (totalmente) inspirado pela série do youtuber [Dave Poo](https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37), onde ele monta passo a passo o as funcionalidades
do emulador. Decidi então expandir o escopo, adicionando uma interface com o emulador, permitindo o usuário inserir um programa Assembly e ver o seu resultado, junto com o passo a passo dentro do sistema
(CPU, Memória, I/O, etc), e criar uma base para construção de um emulador completo de um computador baseado em 6502, para mim e para quem quiser também.

## Objetivo
Está planejado para o projeto:
- Um emulador rápido, feito em C++
- O processador 6502, com todas suas intruções base
- 64 Kb de memória total acessível (sem ROM ou I/O predefinidos)
  - 0x0000 -> 0x00FF como Pagina Zero
  - 0x0100 -> 0x01FF como Pilha
- Um programa para carregar o código Assembly para o emulador
- Uma interface (talvez gráfica) para visualizar o sistema
- Rotinas de teste, para verificar a fidelidade da emulação

## Situação
- Instruções aceitas (em todos os modos):
  - LDA, LDX, LDY, STA, STX, STY, INC, DEC
  - ADC, SBC AND, ORA, EOR, ASL, LSR, ROL, ROR
  - CLC, SEC, CLD, SED, CLI, SEI, CLV
  - JSR, JMP, RTI, RTS, BRK, NOP
  - TSX, TXS, PHA, PHP **(oh não)**, PLA, PLP
  - BIT

- Um Assembler para converter o texto em assembly do 6502 em código de máquina (arquivo binário)
  Formato de uso
  ``` console
  Assembler <arquivo_de_entrada> <arquivo_de_saida*>
  ```
  **opcional (o arquivo de saída será "a.bin", no mesmo diretório do programa)*
  *Testes em andamento!*
  
- Um Emulador para executar o código
  Formato de uso
  ``` console
  Emulator <arquivo_de_entrada> <variante_do_chip*>
  ```
  **opcional (será assumido o chip 6502 base). Veja mais no arquivo "Base.h"*

Em resumo, o projeto está um um estágio pré-incial, com quase todas as instruções completas, e uma interface básica para os programas

## Créditos
Excluindo a biblioteca GoogleTest (é claro), todo o código neste projeto foi feito por mim, CostaCesar. Ainda assim, como deixei claro na sessão [sobre](#sobre), a ideia para esse
emulador veio de [Dave Poo](https://www.youtube.com/playlist?list=PLLwK93hM93Z13TRzPx9JqTIn33feefl37) e sua implementação original do 6502. Esse repositório é apenas uma expansão disso.

Também quero agradecer a [CodingWithMat](https://www.youtube.com/@codingwithmat) pelo tutorial de como usar GoogleTest + Cmake, pois foi de grande ajudar para o projeto.

E se você quer contribuir de alguma forma, pode me chamar no [Github](https://github.com/CostaCesar) ou por [Email](mailto:caiocaesarmcosta@gmail.com).

❤️ Qualquer ajuda é sempre bem vinda! ❤️
