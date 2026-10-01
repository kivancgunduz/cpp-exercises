# cpp-exercises

I've written Python for years. C++ I mostly read and rarely wrote. This repo is where I'm fixing that, aimed at computer vision and embedded work because that's where I need it.

It started as interview prep and turned into a longer plan. Each file compiles on its own with a C++17 compiler. There's no build system and no external libraries, OpenCV included.

## What's here

```
check.hpp                tiny test helper: CHECK, CHECK_NEAR, report()
run.sh                   build and run one file (Linux, macOS, WSL)
run.bat                  the same for Windows
phase1_basics/
  cheatsheet.cpp         the main file, 17 sections (table below)
  NOTES.md               Python to C++ notes
  ex1_1_stats.cpp        mean, median, standard deviation
  ex1_2_wordfreq.cpp     word counts and top-k
  ex1_3_bug_hunt.cpp     five bugs that Python habits cause in C++
```

The phase2 to phase5 folders exist on my machine but are still empty, so GitHub doesn't show them yet.

## Building and running

You need a C++17 compiler: g++ 9 or newer, clang 10 or newer, or Visual Studio 2019 or newer.

On Linux, macOS or WSL:

```bash
./run.sh phase1_basics/cheatsheet.cpp           # every section
./run.sh phase1_basics/cheatsheet.cpp 7         # only section 7
./run.sh phase1_basics/cheatsheet.cpp 7 ex      # section 7 and its exercises
./run.sh phase1_basics/cheatsheet.cpp all ex    # everything
```

`run.sh` builds with AddressSanitizer and UBSan. If I read past the end of an array or touch freed memory, the program stops and shows the line, instead of quietly printing garbage. I'd keep them on while learning.

On Windows:

```bat
run.bat phase1_basics\cheatsheet.cpp 7 ex
```

It uses `g++` if it's on your PATH (MinGW builds don't support the sanitizers). Otherwise it uses `cl` from a Visual Studio Developer Command Prompt.

Every run ends with a line like `[ OK ] 288 passed, 0 failed`. A failing check prints its line number.

## The cheatsheet

`cheatsheet.cpp` is one big file on purpose. Each section is a namespace with example code (Python equivalents in the comments), a `demo()` that checks its own claims with `CHECK`, and an `exercises()` function that tests code I still have to write.

The exercises fail until I implement them. With all the TODOs still empty, `all ex` reports 69 failures. I'm working that down to zero, roughly a section a day.

| # | Topic |
|---|---|
| 01 | Types, variables, const, casts, input/output |
| 02 | if / else, switch, ternary, enum class |
| 03 | Loops (17 kinds) |
| 04 | Functions and lambdas |
| 05 | Strings |
| 06 | Arrays, pointers, references |
| 07 | vector, deque, list |
| 08 | map, unordered_map, set |
| 09 | stack, queue, priority_queue, pair, tuple, optional, variant |
| 10 | Structs and classes |
| 11 | Inheritance and polymorphism |
| 12 | Operator overloading |
| 13 | Memory, RAII, smart pointers, move semantics |
| 14 | Templates |
| 15 | STL algorithms |
| 16 | Exceptions, math, file I/O |
| 17 | Matrix class and image operations |

Section 17 is the one closest to my day job. It has a `Matrix<T>` class (row-major storage, matrix product, transpose), 3x3 matrices for rotating, scaling and translating points, convolution with box, Gaussian and Sobel kernels, and a small filter pipeline built from polymorphic stages.

## Next

- phase2_modern: writing RAII by hand, move semantics, smart pointers, templates
- phase3_cv: image processing without OpenCV (convolution, bilinear resize, integral images, connected components, IoU and NMS)
- phase4_embedded: cache behaviour, fixed-point math, a lock-free ring buffer, a thread-safe frame queue
- phase5_drill: timed problems

This is a personal learning repo. Borrow whatever helps.
