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

open coverage/index.html