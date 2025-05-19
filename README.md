## Table of Contents

1. Project Overview
    
2. Technology Stack
    
3. Compilation & Execution Flow
    
4. Building
    
5. Sample Program
    
6. Project Structure
    
7. Contributing
    
8. License
---

## Project Overview

This project implements a two-phase compiler/interpreter for the **Star Programming Language** in C. It consists of a **lexical analysis** stage, which tokenizes `.sta` source files, and a **semantic analysis & execution** stage, which interprets the token stream, manages symbol tables for integers and text, and supports fixed-count loops, basic arithmetic/string operations, and console I/O.

## File Structure

text

CopyEdit

`. └── PL Source Code                # Root of all source files :contentReference[oaicite:0]{index=0}     ├── CMakeLists.txt           # CMake build script :contentReference[oaicite:1]{index=1}     ├── main.c                   # Lexer + interpreter implementation :contentReference[oaicite:2]{index=2}     ├── code.sta.txt             # Sample SVPL source program     └── cmake-build-debug/       # IDE–generated build directory (can be ignored or removed)         ├── code.sta.txt         # Copied here for debugging         ├── code.lex.txt         # Generated token file         └── untitled2.exe        # Compiled executable`

## Prerequisites

- **CMake ≥ 3.27**: required to process the `CMakeLists.txt` (minimum version set by `cmake_minimum_required(VERSION 3.27)`) [GitHub](https://github.com/SpongeBall-GumPants/Star-Compiler-with-C/commit/734455fbf18a610704b7c5c494424626a4492fdc)[cmake.org](https://cmake.org/cmake/help/latest/command/cmake_minimum_required.html?utm_source=chatgpt.com)
    
- **ISO C11-compliant compiler** (e.g. GCC, Clang): code uses C11 features (`__STDC_VERSION__ == 201112L`) [Wikipedia](https://en.wikipedia.org/wiki/C11_%28C_standard_revision%29?utm_source=chatgpt.com)
    
- **Build tools**: Ninja or Make (selected by CMake) [Wikipedia](https://en.wikipedia.org/wiki/CMake?utm_source=chatgpt.com)
    

## Building

1. **Clone the repository**
    
    `git clone https://github.com/SpongeBall-GumPants/Star-Compiler-with-C.git cd "Star-Compiler-with-C/PL Source Code"`
    
2. **Create a build directory**
    
    `mkdir build && cd build`
    
3. **Generate native build files**
    
    ``cmake ..               # invokes `cmake_minimum_required()` and reads CMakeLists.txt :contentReference[oaicite:7]{index=7}``
    
4. **Compile**
    
    ``cmake --build .        # builds the `untitled2` executable``
    

## Language Features

- **Declarations**:
    
    - `int <identifier> is <IntConst> .`
        
    - `text <identifier> is "<string>" .`
        
- **Assignments**: `<identifier> is <expression> .`
    
- **Loops**: `loop <IntConst> times { … }` (fixed iteration count)
    
- **Input/Output**:
    
    - `read(identifier)` reads from stdin;
        
    - `write(...)` prints comma-separated strings or variable values;
        
    - `newLine` prints a line break.
        
- **Operators**: `+`, `-`, `*`, `/` for integers; `+` (concatenation), `-` (removal) for text.
    

## Sample Program (`code.sta.txt`)

`int myVar is 42 .
`text myText is "Hello, world! ou" .
`loop 3 times {
`   write myText , myVar .
`}
`write "Done!" .

`**Expected Output:** `

`Hello, world! ou	42 Hello, world! ou	   42 Hello, world! ou	  42 Done!`

## Contributing

Contributions are welcome via issues and pull requests. Possible improvements include:

- Adding new language constructs (e.g., conditionals, functions).
    
- Enhancing error reporting and diagnostics.
    
- Refactoring for performance or clearer architecture.
    

---

## License

Released under the **MIT License**.
