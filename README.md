# Advent of Code - C++

This repository contains a C++ template for [Advent of Code](https://adventofcode.com), supporting multiple years.

## Features

- **Multi-year support** - Organize solutions by year (2025, 2024, etc.)
- **CMake-based build system** - Easy compilation and dependency management
- **Google Test integration** - Unit tests for each day's solution
- **Automatic input fetching** - Download puzzle inputs directly from adventofcode.com
- **Performance measurement** - High-precision execution time tracking
- **Modular structure** - Each day is a separate module with its own tests
- **Day generator script** - Automatically scaffold new day solutions
- **Google C++ style enforcement** - clang-format integration

## Project Structure

- **Root Files:**
  - `CMakeLists.txt` - Root build configuration
  - `.clang-format` - Google C++ style configuration
  - `.clang-format-ignore` - Exclude directories from formatting
  - `main.cpp` - Main runner with performance measurement
  - `.env.example` - Template for AOC session cookie

- **scripts/** - Build and generation scripts
  - `generate_day.sh` - Day scaffolding generator
  - `check_format.sh` - Format verification script
  - **templates/day/** - Template files for day generation
    - `solution.h.template`
    - `solution.cpp.template`
    - `test.cpp.template`
    - `CMakeLists.txt.template`
    - `input.txt.template`
    - `input_test.txt.template`

- **src/** - Source code organized by year
  - **common/** - Shared utilities
    - `utils.h/cpp` - String parsing, file I/O helpers
    - `input_fetcher.h/cpp` - AOC input downloader
  - **YYYY/** (e.g., 2025, 2024) - Year-specific solutions
    - **day_N/** - Individual day solutions
      - `CMakeLists.txt` - Build configuration
      - `solution.h/cpp` - part1() and part2() implementations
      - `test.cpp` - Google Test unit tests
      - **tests/** - Input files
        - `input_test.txt` - Example input from puzzle
        - `input.txt` - Real puzzle input

- **build/** - Generated build directory (not in version control)

## Prerequisites

- **C++17 compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.14+**
- **libcurl** (for input fetching)
- **clang-format** (optional, for code formatting)

### Installing Dependencies

**macOS:**

```bash
brew install cmake curl clang-format
```

**Ubuntu/Debian:**

```bash
sudo apt-get install cmake libcurl4-openssl-dev clang-format
```

**Arch Linux:**

```bash
sudo pacman -S cmake curl clang
```

## Setup

1. **Clone or navigate to this repository**

2. **Configure AOC session cookie** (for automatic input fetching):

   ```bash
   cp .env.example .env
   ```

   Edit `.env` and add your session cookie:
   - Go to [adventofcode.com](https://adventofcode.com) and log in
   - Open browser DevTools (F12) → Application/Storage → Cookies
   - Copy the value of the `session` cookie
   - Paste it in `.env`:

     ```
     AOC_SESSION=your_session_cookie_here
     ```

3. **Build the project:**

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Setup LSP/clangd support** (for editor integration):

   The project is already configured to export compile commands. After building, create a symlink in the project root:

   ```bash
   ln -sf build/compile_commands.json compile_commands.json
   ```

   This enables clangd (used by VSCode, Neovim, etc.) to provide proper code completion and diagnostics.

## Usage

### Command-Line Options

```bash
./aoc [options]

Options:
  -y, --year YEAR    Specify year (default: 2025)
  -d, --day DAY      Specify day (1-25, default: all days)
  -p, --part PART    Specify part (1-2, default: both parts)
  -h, --help         Show help message
```

### Examples

```bash
# Run all days of 2025 (default year)
./aoc

# Run all days of 2024
./aoc --year 2024

# Run day 1 of 2025
./aoc -d 1

# Run day 5 of 2024
./aoc -y 2024 -d 5

# Run day 10, part 2 only
./aoc -d 10 -p 2

# Run 2024 day 5 part 2
./aoc --year 2024 --day 5 --part 2
```

The runner will:

- Automatically fetch inputs if not present (requires session cookie in `.env`)
- Run the specified solutions
- Display results in a formatted table with execution times

### Running Tests

```bash
# Run all tests
cd build
ctest --output-on-failure

# Run specific day tests
./2025_day_1_test
./2024_day_1_test
```

## Adding a New Day

### Automated Method (Recommended)

Use the day generator script:

```bash
./scripts/generate_day.sh <year> <day>
```

**Example:**

```bash
./scripts/generate_day.sh 2025 5
```

This will:

1. Create the directory structure: `src/2025/day_5/`
2. Generate all necessary files (solution.h, solution.cpp, test.cpp, CMakeLists.txt)
3. Create placeholder input files
4. Update `main.cpp` with includes and runner calls
5. Ready to rebuild and implement!

**Next steps after generation:**

Note: the generation script is already doing this now.

```bash
cd build
cmake ..
make
./aoc -y 2025 -d 5  # Fetch input and run
```

## Code Style Enforcement

This project uses **Google C++ Style Guide** enforced via clang-format.

### Format Code

```bash
# Auto-format all C++ files
cd build
make format
```

### Check Formatting

```bash
# Verify formatting without modifying files (useful for CI)
cd build
make check-format
```

The `.clang-format` file in the project root defines the formatting rules. All code should be formatted before committing.

## Utility Functions

The `common/utils.h` library provides helpful functions:

- `readFile(filepath)` - Read entire file as string
- `readLines(filepath)` - Read file as vector of lines
- `split(str, delimiter)` - Split string by character
- `splitWhitespace(str)` - Split by any whitespace
- `trim(str)` - Remove leading/trailing whitespace
- `toInt(str)`, `toLong(str)`, `toLongLong(str)` - String to number conversion

## Example Output

```
Advent of Code - C++ Solutions
================================
Running all available solutions for year 2025

+=======+======+======+==================+===============+
| Year  | Day  | Part |      Answer      |   Time (ms)   |
+=======+======+======+==================+===============+
|  2025 |    1 |    1 |            54321 |         0.145 |
|  2025 |    1 |    2 |            98765 |         0.203 |
|  2025 |    2 |    1 |             1337 |         1.256 |
|  2025 |    2 |    2 |             9001 |         2.103 |
+=======+======+======+==================+===============+
| Total time:         3.707 ms                           |
+=======+======+======+==================+===============+
```
