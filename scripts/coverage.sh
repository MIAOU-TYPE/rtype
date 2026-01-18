#!/bin/bash

rm -fr coverage
mkdir -p coverage
cmake --build build -j
ctest --test-dir build
gcovr -r . --object-directory build \
  --exclude '.*vcpkg_installed/.*' \
  --exclude '.*server/tests/.*' \
  --exclude '.*tests/.*' \
  --exclude '.*.hpp' \
  --exclude ".*server/src/server/.*" \
  --html --html-details -o coverage/index.html

if command -v xdg-open >/dev/null 2>&1; then
  xdg-open coverage/index.html
elif command -v open >/dev/null 2>&1; then
  open coverage/index.html
else
  echo "Coverage report generated at coverage/index.html. Please open it in your browser."
fi