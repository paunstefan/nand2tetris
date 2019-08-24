# nand2tetris

My nand2tetris course files.

All developed in C.

# Hack Assembler

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

# Jack Compiler
Compiles Jack to VM code.

To compile run:
```
gcc -o jack_compiler main.c hashtable.c tokenizer.c vmwriter.c compilation_engine.c
```

# To do

*  Add makefiles

# Misc

[Nand2Tetris website](https://www.nand2tetris.org/)