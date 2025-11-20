##
## EPITECH PROJECT, 2025
## rtype
## File description:
## tidy-all
##

#!/bin/bash
set -euo pipefail

mkdir -p build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. >/dev/null
ln -sf compile_commands.json ..

for f in $(find ../client/src ../server/src -name "*.cpp"); do
    clang-tidy "$f" --quiet --fix --
done
