# How this project is structured:
```
project-euler-cpp/
├─ CMakeLists.txt
├─ include/
│  └─ euler/
│     ├─ problem.hpp                # interface + registry (Strategy + Factory Method)
│     └─ util.hpp                   # common helpers (gcd, sieve, etc.)
├─ src/
│  ├─ main.cpp                      # CLI runner (run one or all problems)
│  ├─ common/
│  │  └─ util.cpp
│  └─ problems/
│     ├─ CMakeLists.txt             # adds each pNNN subdir
│     ├─ p001/
│     │  ├─ CMakeLists.txt
│     │  └─ p001.cpp
│     └─ p002/
│        ├─ CMakeLists.txt
│        └─ p002.cpp
├─ tests/                           # tests for utils
├─ benchmarks/                      # google-benchmark
├─ data/                            # (optional) inputs for some problems
├─ scripts/
│  └─ new_problem.sh                # scaffolds a new pNNN
└─ README.md
```

Each problem sits in src/problems/pNNN (zero-padded width 3) and compiles as a tiny object library.

A single euler runner links all problem objects and picks which Strategy (problem) to run at runtime via a Registry + Factory Method.

Shared math goes in euler_common, so we don’t duplicate sieves, prime tests, big-int adapters, etc.

Each problem subdir adds an object library and bubbles its objects up

- Workflow
    - Add a new solution: `./scripts/new_problem.sh` and follow the prompts → edit `src/problems/p042/p042.cpp.`
    - Build: `cmake -S . -B build && cmake --build build -j`.
    - Run one problem: `./build/euler 42`.
    - List problems: `./build/euler --list`.
    - Run all problems: `./build/euler --all`.

# What does the output look like
- Outputs follow the pattern: Problem # — Problem Name — Answer — Status (pass/fail) — Execution time
- <img width="658" height="330" alt="Screenshot 2026-01-11 at 3 14 47 PM" src="https://github.com/user-attachments/assets/1873f72f-94b0-497c-9a15-6b62f7835ea5" />
