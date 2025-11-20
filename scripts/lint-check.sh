#!/bin/bash
##
## EPITECH PROJECT, 2025
## rtype
## File description:
## lint-check
##

set -euo pipefail

# lint-check.sh - C++ code linter and tester for R-Type project

RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[1;33m"
BLUE="\033[0;34m"
NC="\033[0m"

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# Check required tools
for tool in clang-format clang-tidy cmake; do
    command -v "$tool" >/dev/null 2>&1 || { echo -e "${RED}Error: $tool is not installed${NC}"; exit 1; }
done

echo -e "${BLUE}R-Type Lint Check${NC}"
echo "==================================="

cd "$PROJECT_ROOT"

# 1. Format check
echo -e "${BLUE}Checking format (clang-format)...${NC}"
FORMAT_ERRORS=0

for f in $(find client/src server/src \( -name "*.cpp" -o -name "*.hpp" \)); do
    if ! clang-format --dry-run --Werror "$f"; then
        FORMAT_ERRORS=1
        echo -e "${YELLOW}File needing format: $f${NC}"
        diff -u "$f" <(clang-format -style=file "$f") || true
    fi
done

if [ "$FORMAT_ERRORS" -eq 0 ]; then
    echo -e "${GREEN}Format : OK${NC}"
else
    echo -e "${RED}Format : errors detected${NC}"
fi
echo

# 2. Clang-tidy analysis
echo -e "${BLUE}Static analysis (clang-tidy)...${NC}"
mkdir -p build
cd build

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. >/dev/null
if [ ! -f compile_commands.json ]; then
    echo -e "${RED}Error: compile_commands.json not found. CMake configuration may have failed.${NC}" >&2
    exit 1
fi
ln -sf compile_commands.json ..

TIDY_ERRORS=0
for f in $(find ../client/src ../server/src -name "*.cpp"); do
    if ! clang-tidy "$f" --quiet --; then
        TIDY_ERRORS=1
    fi
done

if [ "$TIDY_ERRORS" -eq 0 ]; then
    echo -e "${GREEN}clang-tidy : OK${NC}"
else
    echo -e "${RED}clang-tidy : errors detected${NC}"
fi
echo

echo "==================================="
if [ "$FORMAT_ERRORS" -eq 0 ] && [ "$TIDY_ERRORS" -eq 0 ]; then
    echo -e "${GREEN}All checks passed${NC}"
else
    echo -e "${RED}Errors detected${NC}"
    echo -e "${YELLOW}Please run './scripts/lint-fix.sh' to automatically fix some issues.${NC}"
    exit 1
fi

exit 0
