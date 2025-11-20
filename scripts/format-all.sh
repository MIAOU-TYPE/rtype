##
## EPITECH PROJECT, 2025
## rtype
## File description:
## format-all
##

#!/bin/bash
set -euo pipefail

echo "Formatting all C++ source files..."

find client/src server/src -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i

echo "Formatting completed."