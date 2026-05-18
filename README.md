# CVM - Stack-Based Virtual Machine & Compiler

This project implements a custom stack-based virtual machine and compiler in standard C++20, following recursive descent parsing and lexing principles.

## Features
- **Data Types**: Integers (`int64_t`) and Booleans.
- **Operations**: `+`, `-`, `*`, `/`, `==`, `<`.
- **Variables**: `let` for declaration, `=` for assignment.
- **Control Flow**: `if`/`else` and `while` loops.
- **I/O**: `print` and `input` keywords.

## Structure
- `Lexer`: Converts raw source code into an array of `Token`s.
- `Parser`: Consumes tokens to produce an Abstract Syntax Tree (AST) using recursive descent.
- `Compiler`: Traverses the AST and emits flat bytecode (Opcodes).
- `VM`: Executes the bytecode using a stack-based evaluation loop.

## How to Build
You need CMake and a C++17 compatible compiler.

1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
2. Run CMake:
   ```bash
   cmake ..
   cmake --build .
   ```

## How to Run
After building, an executable named `cvm` (or `cvm.exe` on Windows) will be generated.
You can run it by passing a script file as an argument.

```bash
./cvm ../script.cvm
```

## Packaging for your team
Since all files are currently contained in this folder, you can create a simple ZIP archive for distribution to your team. 

On Windows (PowerShell):
```powershell
Compress-Archive -Path ./* -DestinationPath CVM_Project.zip
```

On Linux/macOS:
```bash
zip -r CVM_Project.zip ./*
```
