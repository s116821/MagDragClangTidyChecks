# MagDrag Clang-Tidy Checks

Custom clang-tidy checks for enforcing MagDrag C++ coding standards.

## Requirements

- LLVM/Clang development packages
- CMake 3.13.4 or higher
- C++17 compiler

### Installing Dependencies

#### Ubuntu/Debian
```
sudo apt-get install llvm-dev libclang-dev clang cmake
```

#### macOS
```
brew install llvm cmake
```

## Building
```
mkdir build && cd build
cmake ..
make
```

This will create a plugin library in `build/lib/`.

## Usage

1. Set the environment variable to point to the plugin:
```
# Linux
export CLANG_TIDY_PLUGIN_PATH=/path/to/build/lib/MagDragNamingCheck.so

# macOS
export CLANG_TIDY_PLUGIN_PATH=/path/to/build/lib/MagDragNamingCheck.dylib
```

2. Create a `.clang-tidy` file in your project root with:
```
Checks: >
  *,
  magdrag-naming-check,
  -fuchsia-*,
  -google-*,
  -zircon-*,
  -abseil-*,
  -modernize-use-trailing-return-type,
  -llvm-*,
  -llvmlibc-*

CheckOptions:
  - key: readability-identifier-naming.ClassCase
    value: CamelCase
  - key: readability-identifier-naming.StructCase
    value: CamelCase
  - key: readability-identifier-naming.EnumCase
    value: CamelCase
  - key: readability-identifier-naming.EnumSuffix
    value: E
  - key: readability-identifier-naming.TypedefCase
    value: CamelCase
  - key: readability-identifier-naming.TypedefSuffix
    value: T
  - key: readability-identifier-naming.FunctionCase
    value: camelCase
  - key: readability-identifier-naming.VariableCase
    value: lower_case
  - key: readability-identifier-naming.GlobalVariableCase
    value: UPPER_CASE
  - key: readability-identifier-naming.MacroDefinitionCase
    value: UPPER_CASE
```

3. Run clang-tidy:
```
clang-tidy your_file.cpp
```

## Naming Conventions Enforced

- Class/struct variables: `variable_name__O`
- Container variables: `container_name__C`
- Enum variables: `enum_name__E`
- Typedef variables: Resolved to either `__O` or `__C` based on underlying type
- Global variables: `GLOBAL_NAME`
- Function names: `camelCase`
- Class/struct declarations: `PascalCase`
- Enum declarations: `PascalCaseE`
- Typedef declarations: `PascalCaseT`

## Integration Guide

For instructions on how to integrate these checks into your repository, see [INTEGRATION.md](INTEGRATION.md).
