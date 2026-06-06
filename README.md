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


## Setup

**macOS:** install brew LLVM and point CMake at it:
```bash
brew install llvm
cmake -B build -DCMAKE_CXX_COMPILER=$(brew --prefix llvm)/bin/clang++
```

**Linux:** GCC 13+ or Clang 17+:
```bash
cmake -B build
```


## Building

**One part:** (e.g. part 1 of day 1 of 2025)
```bash
cmake --build build --target 2025_day01_part1
```

**Everything:**
```bash
cmake --build build
```


## Running

**One part:** (e.g. part 1 of day 1 of 2025)
```bash
./build/2025_day01_part1
```

**Everything:**
```bash
find build -maxdepth 1 -name "*_day*_part*" | sort | xargs -I{} sh -c 'echo "{}:" && {}'
```


## Adding a new day

I was repeating the same process to add new days to my solutions and fetching the input a lot; so, I made a script:

```bash
./new-day.sh YEAR DAY
```

It creates `part1.cpp` and `part2.cpp` and fetches `input.txt` from [adventofcode.com](https://adventofcode.com) if a session token is set up (see below).

## Getting your session token

Log in to [adventofcode.com](https://adventofcode.com), then:
- **Firefox:** DevTools (`Cmd+Option+I`/`F12`) > Storage > Cookies > https://adventofcode.com > copy the session value
- **Chrome:** DevTools (`Cmd+Option+I`/`F12`) > Application > Cookies > https://adventofcode.com > copy the session value

Store it in your project root:
```bash
echo "YOUR_TOKEN" > .aoc_session
```
Or temporarily as an environment variable:
```bash
export AOC_SESSION="YOUR_TOKEN"
```

