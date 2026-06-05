# Advent of Code

My solutions to [Advent of Code](https://adventofcode.com), written in C++23.


## Progress

<!--- advent_readme_stars table --->


## Structure

```
advent-of-code/
├── utils/
├── 2025/
│   ├── day01/
│   │   ├── part1.cpp
│   │   ├── part2.cpp
│   │   └── input.txt
│   └── ...
└── ...
```


## Building

**Prerequisites (macOS):**

Install LLVM with Homebrew: `brew install llvm`

**One part:**
```bash
cmake --build build --target 2025_day01_part1
```

**Everything:**
```bash
cmake -B build && cmake --build build
```


## Running

**One part:**
```bash
./build/2025/2025_day01_part1
```

**Everything:**
```bash
find build -name "2025_day*" -perm +111 | sort | xargs -I{} sh -c 'echo "{}:" && {}'
```