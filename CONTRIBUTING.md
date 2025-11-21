# Contributing to R-Type

Thank you for your interest in contributing to R-Type! This document outlines the guidelines for contributing to the project.

## Branching Model

We follow a simplified Git Flow branching model:

- `main`: The main branch containing the latest stable release.
- Feature branches: Created from `main` for new features or bug fixes, named as `feature/feature-name` or `fix/bug-description`.

Always create pull requests to merge feature branches into `main`.

## Commit Message Conventions

We use conventional commits to ensure clear and consistent commit messages. Each commit message should follow this format:

```
<type>: <description>
```

Types:
- `feat`: A new feature
- `fix`: A bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, etc.)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

Examples:
- `feat: add user login functionality`
- `fix: resolve connection timeout issue`

## How to Run Tests

To run the test suite:

1. Build the project:
   ```bash
   mkdir -p build
   cd build
   cmake ..
   make
   ```

2. Run tests:
   ```bash
   ctest
   ```

Or run tests directly from the build directory:
```bash
cd build
ctest
```

Ensure all tests pass before submitting a pull request.

## Code Quality Standards

Before opening a merge request, please ensure your code adheres to our quality standards by running the following scripts:

### Automatic Code Formatting and Fixing

Run the lint-fix script to automatically format your code and fix common issues:

```bash
./scripts/lint-fix.sh
```

This script will:
- Apply clang-format to all C++ source and header files in `client/src/` and `server/src/`
- Run clang-tidy with auto-fix on all C++ source files

### Verify Code Quality

After running the fix script, verify that all checks pass:

```bash
./scripts/lint-check.sh
```

This script will:
- Check that all files are properly formatted with clang-format
- Run clang-tidy static analysis on all C++ source files

If any errors are reported, please fix them before submitting your merge request.

## CI Expectations Before Merging

Before merging a pull request, the Continuous Integration (CI) pipeline must pass all checks:

1. **Linting**: Runs `./scripts/lint-check.sh` to ensure code formatting and static analysis pass.
2. **Build**: Compiles the project using CMake and Make to verify no build errors.
3. **Tests**: Executes the test suite with `ctest` to ensure all tests pass.
4. **Integration Tests**: Runs any additional integration or end-to-end tests if applicable.

If CI fails, review the logs, fix the issues, and push the changes. Do not merge until all CI checks are green.

## Development Setup

Make sure you have the following tools installed:
- clang-format
- clang-tidy
- CMake 3.20 or later
- A C++20 compatible compiler

For any questions, please reach out to the development team.