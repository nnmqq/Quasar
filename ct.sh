#!/bin/bash

# Configuration
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SOURCE_DIR="src"
COMPILE_COMMANDS_DIR="./" # Directory containing compile_commands.json

# Print help message
print_help() {
    echo "Usage: $0 [options] [file]"
    echo
    echo "Options:"
    echo "  -h, --help                 Show this help message and exit."
    echo
    echo "If no file is specified, all .c files in $SOURCE_DIR will be analyzed."
    echo "If a file is specified, only that file will be analyzed."
}

# Parse arguments
if [ "$#" -eq 1 ]; then
    case "$1" in
        -h|--help)
            print_help
            exit 0
            ;;
        *)
            FILE="$1"
            ;;
    esac
elif [ "$#" -eq 0 ]; then
    FILE=""
else
    echo "Error: Too many arguments."
    print_help
    exit 1
fi
# Check if compile_commands.json exists
if [ ! -f "$COMPILE_COMMANDS_DIR/compile_commands.json" ]; then
    echo "Error: compile_commands.json not found in $COMPILE_COMMANDS_DIR."
    exit 1
fi

# Run clang-tidy
if [ -z "$FILE" ]; then
    echo "Running clang-tidy on all .c files..."
    find "$SOURCE_DIR" -name '*.c' | while read -r file; do
        echo "Analyzing $file"
        clang-tidy "$file" --config-file=$DIR/.clang-tidy -header-filter=.*
    done
else
    if [ ! -f "$FILE" ]; then
        echo "Error: File $FILE not found."
        exit 1
    fi
    echo "Analyzing $FILE"
    clang-tidy "$FILE" --config-file=$DIR/.clang-tidy -header-filter=.*
fi

echo "clang-tidy analysis completed."
