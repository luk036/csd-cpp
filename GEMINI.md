# Gemini Code Assistant Context: `csd-cpp`

This document provides context for the `csd-cpp` C++ project.

## Project Overview

This is a C++ library for converting numbers between decimal format and Canonical Signed Digit (CSD) representation. CSD is a way of writing numbers using only the symbols `0`, `+`, and `-`, which is efficient for certain digital signal processing applications.

The project is structured as a modern C++ template project, using CMake as its primary build system and also providing support for `xmake`. It emphasizes clean separation between the library code, tests, and standalone executables.

**Key Technologies & Libraries:**
- **Language:** C++ (C++11 standard)
- **Build Systems:** CMake (primary), xmake
- **Dependencies:** `fmtlib/fmt`
- **Dependency Management:** `CPM.cmake`
- **Testing Framework:** `doctest`
- **Code Formatting:** `clang-format` and `cmake-format`
- **CI/CD:** GitHub Actions
- **Documentation:** Doxygen

**Core Logic:**
- The main library interface is defined in `include/csd/csd.hpp`.
- The implementation is in `source/csd.cpp`.
- The public API provides functions like `to_csd`, `to_decimal`, and variants for integers and for specifying the number of non-zero digits.

## Building and Running

The project contains several components (library, tests, standalone app) and can be built using either CMake or xmake.

### Building with CMake (Recommended)

The `README.md` provides detailed instructions. The most common commands are summarized below. It is recommended to run CMake from a separate `build` directory.

**1. Configure the Build (from project root):**
```bash
# Configure to build and run tests
cmake -S test -B build/test

# Or, configure to build everything at once (recommended for IDEs)
cmake -S all -B build
```
- Add `-DENABLE_TEST_COVERAGE=1` to enable code coverage.
- Add `-DCMAKE_BUILD_TYPE=Debug` for a debug build.

**2. Build the Project:**
```bash
cmake --build build/test
# or if you configured with `all`
cmake --build build
```

**3. Run Tests:**
```bash
# After building the 'test' configuration
cd build/test && ctest
# Or simply execute the test binary
./build/test/CsdTests
```

**4. Run the Standalone App:**
```bash
# If you built using the 'all' or 'standalone' configuration
./build/standalone/Csd --help
```

### Building with xmake

The project can also be built and tested using `xmake`.

```bash
# Configure for a release build
xmake f -m release -y

# Build the project
xmake

# Run the tests
xmake run test_csd
```

## Development Conventions

### Coding Style

The project uses `clang-format` to enforce a consistent coding style. The configuration is in `.clang-format` and is based on the **Google C++ Style Guide** with a few key overrides:
- **Indentation:** 4 spaces
- **Column Limit:** 100 characters
- **Braces:** Attached to the preceding statement (`Attach` style).

To automatically format the code, run the `fix-format` target after configuring with CMake:
```bash
cmake --build build --target fix-format
```

### Testing

- Tests are written using the **doctest** framework.
- Test files are located in the `test/source/` directory (e.g., `test_csd.cpp`).
- Tests are defined within `TEST_CASE` blocks and use assertions like `CHECK_EQ`.
- New tests should be added to the `test/source` directory and will be picked up automatically by the build system.

### Dependencies

External dependencies are managed via `CPM.cmake` in the main `CMakeLists.txt`. When adding a new dependency, use the `CPMAddPackage` command. This approach ensures reproducible builds.
