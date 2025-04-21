#!/bin/sh

SRC_DIR="src"

if [ ! -d "$SRC_DIR" ]; then
    echo "Error: Directory '$SRC_DIR' does not exist."
    exit 1
fi

if command -v clang-format >/dev/null 2>&1; then
    LINTER="clang-format"
    LINT_COMMAND="clang-format -i"
    echo "Using clang-format as linter"
elif command -v cpplint >/dev/null 2>&1; then
    LINTER="cpplint"
    LINT_COMMAND="cpplint"
    echo "Using cpplint as linter"
else
    echo "Error: No C++ linter found. Please install clang-format or cpplint."
    exit 1
fi

CPP_FILES=$(find "$SRC_DIR" -type f \( -name "*.cpp" -o -name "*.h" \))

if [ -z "$CPP_FILES" ]; then
    echo "No .cpp or .h files found in '$SRC_DIR' directory."
    exit 0
fi

FILE_COUNT=$(echo "$CPP_FILES" | wc -l)
echo "Found $FILE_COUNT files to lint"

if [ "$LINTER" = "clang-format" ]; then
    echo "$CPP_FILES" | xargs clang-format -i
    echo "All files have been formatted with clang-format"
else
    for file in $CPP_FILES; do
        echo "Linting $file..."
        $LINT_COMMAND "$file"
    done
    echo "All files have been linted with cpplint"
fi

echo "Linting complete!"
