#!/bin/bash
##
## EPITECH PROJECT, 2025
## rtype
## File description:
## lint-fix
##

set -euo pipefail

# lint-fix.sh – Automatic correction of C++ style (clang-format + tidy fix)

RED="\033[0;31m"
GREEN="\033[0;32m"
BLUE="\033[0;34m"
NC="\033[0m"

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# Check required tools
for tool in clang-format clang-tidy cmake; do
    command -v "$tool" >/dev/null 2>&1 || { echo -e "${RED}Error: $tool is not installed${NC}"; exit 1; }
done

echo -e "${BLUE}Auto-Fix C++ code${NC}"
echo "==================================="

cd "$PROJECT_ROOT"

# 1. Automatic formatting
echo -e "${BLUE}Applying format (clang-format)...${NC}"
find client/src server/src \( -name "*.cpp" -o -name "*.hpp" \) -print0 | xargs -0 clang-format -i
echo -e "${GREEN}Format applied${NC}"
echo

# 2. clang-tidy auto-fix
echo -e "${BLUE}Running clang-tidy analysis and fix...${NC}"
mkdir -p build
cd build

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. >/dev/null
if [ ! -f compile_commands.json ]; then
    echo -e "${RED}Error: compile_commands.json not found. CMake configuration may have failed.${NC}" >&2
    exit 1
fi
ln -sf compile_commands.json ..

TIDY_FAILED=0
while IFS= read -r -d '' f; do
    if ! clang-tidy "$f" --quiet -fix --; then
        echo -e "${RED}clang-tidy failed for $f${NC}" >&2
        TIDY_FAILED=1
    fi
done < <(find ../client/src ../server/src -name "*.cpp" -print0)
if [ "$TIDY_FAILED" -eq 1 ]; then
    exit 1
fi

# 3. Check again to ensure all fixes are applied
cd ..
bash "${PROJECT_ROOT}/scripts/lint-check.sh"

echo -e "${GREEN}clang-tidy fix completed${NC}"
echo "==================================="
echo -e "${GREEN}Auto-Fix completed.${NC}"
