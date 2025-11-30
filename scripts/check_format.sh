#!/bin/bash

# Check if C++ files are formatted according to .clang-format
# Returns 0 if all files are formatted, 1 otherwise

set -e

# Get script directory and project root
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"

# Find clang-format
CLANG_FORMAT=$(which clang-format 2>/dev/null || echo "")

if [ -z "$CLANG_FORMAT" ]; then
    echo "Error: clang-format not found in PATH"
    exit 1
fi

# Find all C++ files
CPP_FILES=$(find "$PROJECT_ROOT/src" "$PROJECT_ROOT" -maxdepth 1 -name "*.cpp" -o -name "*.h" 2>/dev/null)

NEEDS_FORMATTING=0

echo "Checking C++ code formatting..."

for file in $CPP_FILES; do
    # Run clang-format and compare with original
    if ! $CLANG_FORMAT -style=file "$file" | diff -q "$file" - > /dev/null; then
        echo "  ✗ $file needs formatting"
        NEEDS_FORMATTING=1
    fi
done

if [ $NEEDS_FORMATTING -eq 0 ]; then
    echo "✓ All files are properly formatted!"
    exit 0
else
    echo ""
    echo "Some files need formatting. Run 'make format' to fix."
    exit 1
fi
