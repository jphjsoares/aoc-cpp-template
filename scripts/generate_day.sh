#!/bin/bash

# Advent of Code Day Generator Script
# Usage: ./scripts/generate_day.sh <year> <day>

set -e

# Check arguments
if [ $# -ne 2 ]; then
  echo "Usage: $0 <year> <day>"
  echo "Example: $0 2025 5"
  exit 1
fi

YEAR=$1
DAY=$2

# Validate inputs
if ! [[ "$YEAR" =~ ^[0-9]{4}$ ]]; then
  echo "Error: Year must be a 4-digit number"
  exit 1
fi

if ! [[ "$DAY" =~ ^[0-9]+$ ]] || [ "$DAY" -lt 1 ] || [ "$DAY" -gt 25 ]; then
  echo "Error: Day must be a number between 1 and 25"
  exit 1
fi

# Get script directory and project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Define paths
TEMPLATE_DIR="$SCRIPT_DIR/templates/day"
YEAR_DIR="$PROJECT_ROOT/src/$YEAR"
DAY_DIR="$YEAR_DIR/day_$DAY"
TESTS_DIR="$DAY_DIR/tests"

# Check if day already exists
if [ -d "$DAY_DIR" ]; then
  echo "Error: Day $DAY for year $YEAR already exists at $DAY_DIR"
  exit 1
fi

echo "Generating day $DAY for year $YEAR..."

# Create directories
mkdir -p "$TESTS_DIR"
echo "Created directory structure: $DAY_DIR"

# Generate solution.h
sed -e "s/{{YEAR}}/$YEAR/g" -e "s/{{DAY}}/$DAY/g" \
  "$TEMPLATE_DIR/solution.h.template" >"$DAY_DIR/solution.h"
echo "Created solution.h"

# Generate solution.cpp
sed -e "s/{{YEAR}}/$YEAR/g" -e "s/{{DAY}}/$DAY/g" \
  "$TEMPLATE_DIR/solution.cpp.template" >"$DAY_DIR/solution.cpp"
echo "Created solution.cpp"

# Generate test.cpp
sed -e "s/{{YEAR}}/$YEAR/g" -e "s/{{DAY}}/$DAY/g" \
  "$TEMPLATE_DIR/test.cpp.template" >"$DAY_DIR/test.cpp"
echo "Created test.cpp"

# Generate CMakeLists.txt
sed -e "s/{{YEAR}}/$YEAR/g" -e "s/{{DAY}}/$DAY/g" \
  "$TEMPLATE_DIR/CMakeLists.txt.template" >"$DAY_DIR/CMakeLists.txt"
echo "Created CMakeLists.txt"

# Generate empty input files
sed -e "s/{{YEAR}}/$YEAR/g" -e "s/{{DAY}}/$DAY/g" \
  "$TEMPLATE_DIR/input_test.txt.template" >"$TESTS_DIR/input_test.txt"

sed -e "s/{{YEAR}}/$YEAR/g" -e "s/{{DAY}}/$DAY/g" \
  "$TEMPLATE_DIR/input.txt.template" >"$TESTS_DIR/input.txt"
echo "Created input files"

# Update main.cpp to include the new day
MAIN_FILE="$PROJECT_ROOT/main.cpp"
if [ -f "$MAIN_FILE" ]; then
  # Check if include already exists
  if ! grep -q "#include \"${YEAR}/day_${DAY}/solution.h\"" "$MAIN_FILE"; then
    # Find the last include line for this year and add after it
    sed -i.bak "/^#include \"${YEAR}\/day_[0-9]*\/solution.h\"/a\\
#include \"${YEAR}/day_${DAY}/solution.h\"
" "$MAIN_FILE" 2>/dev/null || {
      # If no includes for this year exist, add after the last include
      sed -i.bak "/^#include.*solution.h\"/a\\
#include \"${YEAR}/day_${DAY}/solution.h\"
" "$MAIN_FILE"
    }
    echo "Added include to main.cpp"
  fi

  # Add runDay call (find the year comment section and add there)
  if ! grep -q "runDay($YEAR, $DAY," "$MAIN_FILE"; then
    # Try to add after last runDay for this year
    sed -i.bak "/runDay($YEAR, [0-9]*, year_$YEAR::day_[0-9]*::/a\\
    runDay($YEAR, $DAY, year_${YEAR}::day_${DAY}::part1, year_${YEAR}::day_${DAY}::part2);
" "$MAIN_FILE" 2>/dev/null || {
      # If no runDay for this year, add in the runDay section
      sed -i.bak "/\/\/ Year $YEAR/a\\
    runDay($YEAR, $DAY, year_${YEAR}::day_${DAY}::part1, year_${YEAR}::day_${DAY}::part2);
" "$MAIN_FILE"
    }
    echo "Added runDay call to main.cpp"
  fi

  rm -f "$MAIN_FILE.bak"
fi

if [ ! -d "../build" ]; then
  mkdir ../build
fi
pushd build
cmake ..
make
popd

echo ""
echo "✓ Successfully generated day $DAY for year $YEAR!"
echo ""
echo "Next steps:"
echo "  1. Fetch the input (requires .env with AOC_SESSION):"
echo "     ./aoc -y $YEAR -d $DAY"
echo "  2. Implement your solution in:"
echo "     $DAY_DIR/solution.cpp"
echo "  3. Run your solution:"
echo "     ./aoc -y $YEAR -d $DAY"
echo ""
