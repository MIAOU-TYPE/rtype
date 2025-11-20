##
## EPITECH PROJECT, 2025
## rtype
## File description:
## lint-fix
##

#!/bin/bash
set -euo pipefail

# lint-fix.sh – Automatic correction of C++ style (clang-format + tidy fix)

RED="\033[0;31m"
GREEN="\033[0;32m"
BLUE="\033[0;34m"
NC="\033[0m"

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

echo -e "${BLUE}Auto-Fix C++ code${NC}"
echo "==================================="

cd "$PROJECT_ROOT"

# 1. Automatic formatting
echo -e "${BLUE}Applying format (clang-format)...${NC}"
find client/src server/src -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
echo -e "${GREEN}Format applied${NC}"
echo

# 2. clang-tidy auto-fix
echo -e "${BLUE}Running clang-tidy analysis and fix...${NC}"
mkdir -p build
cd build

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. >/dev/null
ln -sf compile_commands.json ..

for f in $(find ../client/src ../server/src -name "*.cpp"); do
    clang-tidy "$f" --quiet -fix -- || true
done

# 3. Check again to ensure all fixes are applied
cd ..
./scripts/lint-check.sh

echo -e "${GREEN}clang-tidy fix completed${NC}"
echo "==================================="
echo -e "${GREEN}Auto-Fix completed.${NC}"
