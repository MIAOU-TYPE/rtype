# Contributing to R-Type

Thank you for your interest in contributing to R-Type! This document outlines the guidelines for contributing to the project.

## Code Quality Standards

Before opening a merge request, please ensure your code adheres to our quality standards by running the following scripts:

### 1. Automatic Code Formatting and Fixing

Run the lint-fix script to automatically format your code and fix common issues:

```bash
./scripts/lint-fix.sh
```

This script will:
- Apply clang-format to all C++ source and header files in `client/src/` and `server/src/`
- Run clang-tidy with auto-fix on all C++ source files

### 2. Verify Code Quality

After running the fix script, verify that all checks pass:

```bash
./scripts/lint-check.sh
```

This script will:
- Check that all files are properly formatted with clang-format
- Run clang-tidy static analysis on all C++ source files

If any errors are reported, please fix them before submitting your merge request.

### 3. Individual Scripts

You can also run individual scripts as needed:

- **Format all files**: `./scripts/format-all.sh`
- **Run clang-tidy fixes**: `./scripts/tidy-all.sh`

## Merge Request Requirements

- All scripts must pass without errors
- Code must compile successfully
- Include a clear description of changes
- Ensure tests pass (if applicable)

## Development Setup

Make sure you have the following tools installed:
- clang-format
- clang-tidy
- CMake 3.20 or later
- A C++20 compatible compiler

For any questions, please reach out to the development team.