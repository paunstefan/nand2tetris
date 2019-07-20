# nand2tetris

My nand2tetris course files.

All developed in C.

# Assembler

Assembles Hack assembly language to machine code.

To compile run:
```
gcc -o hasm assembler.c parser.c code.c hashtable.c
```

# VM Translator
Translates VM code into assembly language.

To compile run:
```
gcc -o vm_translator main.c parser.c codewriter.c
```