# Compiler-Construction-Project
Compiler for an artificial programming language called ERPLAG. This project was built as a part of partial fulfillment of course **Compiler Construction** in the **Spring Semester of the academic year 2019-2020**.


This project has been been implemented in a pipeline manner with stages consisting of:
* Lexer
* Parser
* AST generation
* Type Checking
* Semantic Analysis
* Code Generation (NASM 32-bit compatible assembly).

**Authors**
* [Ayush Vachaspati](https://github.com/AyushVachaspati)
* [Indraneel Ghosh](https://github.com/ighosh98)
* [G. Adityan](https://github.com/Adityan28)

## Usage

```bash
    make
    ./compiler <erplag_file> <asm_output_file>
    nasm -f elf -g <asm_output_file>
    gcc -no-pie -m32 code.o -o <output_filename>
    ./<output_executable>
```
