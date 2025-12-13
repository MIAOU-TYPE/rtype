# Client Unit Tests

This folder contains unit tests for the R-Type client, with a particular focus on the network ping/pong system.

## Structure

```
tests/
├── CMakeLists.txt                      # CMake configuration for tests
└── network/
    ├── MockNetWrapper.hpp              # Network wrapper mock
    ├── TestableNetClient.hpp           # Testable version of NetClient
    └── testNetClientPingPongImpl.cpp   # Ping/pong system tests
```

## Compilation and Execution

### Prerequisites
- CMake 3.20+
- GTest/GMock (installed via vcpkg)
- C++20 compiler

### Compilation
```bash
cd build
cmake .. -DBUILD_TESTING=ON
make client_unit_tests -j$(nproc)
```

### Execution
```bash
# Run all tests
./tests/client_unit_tests

# Run with verbose output
./tests/client_unit_tests --gtest_verbose
```
