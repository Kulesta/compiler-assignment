# Mini Compiler

This project is a mini C-like compiler implemented in C++ as part of a compiler design assignment.

## Features
- Lexical analysis (tokenization)
- Recursive descent parser
- Abstract Syntax Tree (AST)
- Semantic analysis
  - Undefined variable detection
  - Function declaration and call checking
  - Argument count and type validation
  - Return type checking
  - Invalid assignment detection

## Supported Language Constructs
- `int`, `bool`, and `void` types
- Variable declarations
- Function declarations and calls
- Arithmetic expressions
- Return statements
- Block scopes

## Project Structure
- `lexer.*` – Lexical analyzer
- `parser.*` – Recursive descent parser
- `ast.h` – Abstract Syntax Tree definitions
- `semantic.*` – Semantic analysis
- `main.cpp` – Test driver

## Status
All provided test cases pass successfully, including semantic error detection.

## Language
- C++

