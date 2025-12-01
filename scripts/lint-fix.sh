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
cd "$PROJECT_ROOT"

for tool in clang-format-20 clang-tidy-20; do
    command -v "$tool" >/dev/null 2>&1 || { echo -e "${RED}Missing $tool${NC}"; exit 1; }
done

echo -e "${BLUE}Lint Fix (format + tidy)${NC}"
echo "==================================="

###############################################
# 1. APPLY CLANG-FORMAT
###############################################
echo -e "${BLUE}Applying format...${NC}"
find client/src server/src \( -name "*.cpp" -o -name "*.hpp" \) -print0 \
    | xargs -0 clang-format-20 -i

echo -e "${GREEN}Format applied${NC}"
echo

###############################################
# 2. CLANG-TIDY FIX (basic mode)
###############################################
echo -e "${BLUE}Applying clang-tidy fixes...${NC}"

TIDY_FAILED=0
while IFS= read -r -d '' f; do
    if ! clang-tidy-20 "$f" -fix --quiet \
            --extra-arg=-std=c++20 \
            --extra-arg=-Iserver/src \
            --extra-arg=-Iclient/src 2>/dev/null; then
        TIDY_FAILED=1
        echo -e "${RED}clang-tidy failed on $f${NC}"
    fi
done < <(find client/src server/src -name "*.cpp" -not -path "*/tests/*" -print0)

echo -e "${GREEN}clang-tidy fix done${NC}"
echo

###############################################
# RESULT
###############################################
echo "==================================="
if [[ $TIDY_FAILED -eq 0 ]]; then
    echo -e "${GREEN}All auto-fixes applied successfully${NC}"
else
    echo -e "${YELLOW}Some issues could not be fixed automatically${NC}"
fi
