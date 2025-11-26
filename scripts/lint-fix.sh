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
YELLOW="\033[1;33m"
NC="\033[0m"

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# Check required tools
for tool in clang-format-20 clang-tidy-20 cmake; do
    command -v "$tool" >/dev/null 2>&1 || { echo -e "${RED}Error: $tool is not installed${NC}"; exit 1; }
done

echo -e "${BLUE}Auto-Fix C++ code${NC}"
echo "==================================="

cd "$PROJECT_ROOT"

# 1. Automatic formatting
echo -e "${BLUE}Applying format (clang-format)...${NC}"
find client/src server/src \( -name "*.cpp" -o -name "*.hpp" \) -print0 | xargs -0 clang-format-20 -i
echo -e "${GREEN}Format applied${NC}"
echo

# 2. clang-tidy auto-fix
echo -e "${BLUE}Running clang-tidy analysis and fix...${NC}"
rm -rf build
mkdir -p build
cd build

# Configure CMake without tests for static analysis
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBUILD_TESTING=OFF .. >/dev/null
if [ ! -f compile_commands.json ]; then
    echo -e "${RED}Error: compile_commands.json not found. CMake configuration may have failed.${NC}" >&2
    exit 1
fi
ln -sf compile_commands.json ..

cd ..
TIDY_FAILED=0
while IFS= read -r -d '' f; do
    if ! clang-tidy-20 "$f" --quiet -fix -p build/compile_commands.json  --extra-arg=-Wno-unknown-pragmas; then
        echo -e "${RED}clang-tidy failed for $f${NC}" >&2
        TIDY_FAILED=1
    fi
done < <(find client/src server/src -name "*.cpp" -not -path "*/tests/*" -print0)

echo -e "${GREEN}clang-tidy fix completed${NC}"
echo "==================================="
echo -e "${GREEN}Auto-Fix completed.${NC}"

if [ "$TIDY_FAILED" -eq 0 ]; then
    echo -e "${BLUE}Tip: Run './scripts/lint-check.sh' to verify all issues are resolved.${NC}"
else
    echo -e "${YELLOW}Some errors could not be fixed automatically.${NC}"
    echo -e "${YELLOW}Please run './scripts/lint-check.sh' to see remaining issues.${NC}"
fi
