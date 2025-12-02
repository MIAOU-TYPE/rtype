#!/bin/bash
##
## EPITECH PROJECT, 2025
## rtype
## File description:
## lint-fix
##

set -euo pipefail

RED="\033[0;31m"
GREEN="\033[0;32m"
BLUE="\033[0;34m"
NC="\033[0m"

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROJECT_ROOT"

command -v clang-format-20 >/dev/null 2>&1 || {
    echo -e "${RED}Missing clang-format-20${NC}"
    exit 1
}

echo -e "${BLUE}Lint Fix (clang-format only)${NC}"
echo "==================================="

###############################################
# APPLY clang-format
###############################################
find client/src server/src \( -name "*.cpp" -o -name "*.hpp" \) -print0 \
    | xargs -0 clang-format-20 -i

echo -e "${GREEN}Format applied${NC}"
echo "==================================="
echo -e "${GREEN}All auto-fixes applied successfully${NC}"
