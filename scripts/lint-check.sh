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
cd "$PROJECT_ROOT"

# Tools check
for tool in clang-format-20 clang-tidy-20; do
    command -v "$tool" >/dev/null 2>&1 || { echo -e "${RED}Missing $tool${NC}"; exit 1; }
done

echo -e "${BLUE}Lint Check (format + static analysis)${NC}"
echo "==================================="

###############################################
# 1. FORMAT CHECK
###############################################
echo -e "${BLUE}Checking format...${NC}"
FORMAT_ERRORS=0

while IFS= read -r -d '' f; do
    if ! clang-format-20 --dry-run --Werror "$f"; then
        FORMAT_ERRORS=1
        echo -e "${YELLOW}Needs format: $f${NC}"
        diff -u "$f" <(clang-format-20 -style=file "$f") || true
    fi
done < <(find client/src server/src -name "*.cpp" -o -name "*.hpp" -print0)

[[ $FORMAT_ERRORS -eq 0 ]] && echo -e "${GREEN}Format OK${NC}" || echo -e "${RED}Format errors found${NC}"

###############################################
# 2. TIDY CHECK (without compile_commands.json)
###############################################
echo
echo -e "${BLUE}Checking with clang-tidy (basic mode)...${NC}"

TIDY_ERRORS=0
while IFS= read -r -d '' f; do
    # no compile_commands.json needed
    if ! clang-tidy-20 "$f" --quiet --extra-arg=-std=c++20 --extra-arg=-Iserver/src --extra-arg=-Iclient/src 2>/dev/null; then
        TIDY_ERRORS=1
        echo -e "${RED}clang-tidy issue: $f${NC}"
    fi
done < <(find server/src client/src -name "*.cpp" -not -path "*/tests/*" -print0)

[[ $TIDY_ERRORS -eq 0 ]] && echo -e "${GREEN}clang-tidy OK${NC}" || echo -e "${RED}clang-tidy errors found${NC}"

###############################################
# RESULT
###############################################
echo
echo "==================================="

if [[ $FORMAT_ERRORS -eq 0 && $TIDY_ERRORS -eq 0 ]]; then
    echo -e "${GREEN}All checks passed${NC}"
else
    echo -e "${RED}Lint failed${NC}"
    exit 1
fi

exit 0
