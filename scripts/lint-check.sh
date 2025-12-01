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

# Only clang-format is required now
command -v clang-format-20 >/dev/null 2>&1 || {
    echo -e "${RED}Missing clang-format-20${NC}"
    exit 1
}

echo -e "${BLUE}Lint Check (clang-format only)${NC}"
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
    fi
done < <(find client/src server/src -name "*.cpp" -o -name "*.hpp" -print0)

[[ $FORMAT_ERRORS -eq 0 ]] && \
    echo -e "${GREEN}Format OK${NC}" || \
    echo -e "${RED}Format errors found${NC}"

echo "==================================="

if [[ $FORMAT_ERRORS -eq 0 ]]; then
    echo -e "${GREEN}All checks passed${NC}"
else
    echo -e "${RED}Lint failed${NC}"
    exit 1
fi
