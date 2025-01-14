# Integrating MagDrag Clang-Tidy Checks

This guide explains how to integrate these checks into your C++ repository.

## Method 1: Git Submodule (Recommended)

1. Add this repository as a submodule:
```
git submodule add https://github.com/yourusername/MagDragClangTidyChecks.git src/inc/external/MagDragClangTidyChecks
git submodule update --init --recursive
```

2. Add to your CMakeLists.txt:
```
add_subdirectory(external/MagDragClangTidyChecks)
```

3. Copy the `.clang-tidy` configuration from the README.md to your repository root.

4. Set up pre-commit hooks (optional):

Create `.git/hooks/pre-commit`:
```
#!/bin/bash

# Check for clang-format and clang-tidy
if ! command -v clang-format &> /dev/null; then
    echo "clang-format not found. Please install it."
    exit 1
fi

if ! command -v clang-tidy &> /dev/null; then
    echo "clang-tidy not found. Please install it."
    exit 1
fi

# Set plugin path
export CLANG_TIDY_PLUGIN_PATH="$(git rev-parse --show-toplevel)/external/MagDragClangTidyChecks/build/lib/MagDragNamingCheck.so"

# Get all staged C++ files
files=$(git diff --cached --name-only --diff-filter=ACMR | grep -E "\.(cpp|hpp|h|cc|cxx)$")

if [ -n "$files" ]; then
    # Run clang-format
    echo "Running clang-format..."
    for file in $files; do
        clang-format -i "$file"
        git add "$file"
    done

    # Run clang-tidy
    echo "Running clang-tidy..."
    for file in $files; do
        clang-tidy "$file" -fix
        git add "$file"
    done
fi

exit 0
```

Make it executable:
```
chmod +x .git/hooks/pre-commit
```

## Method 2: Manual Installation

1. Clone and build this repository:
```
git clone https://github.com/yourusername/MagDragClangTidyChecks.git
cd MagDragClangTidyChecks
mkdir build && cd build
cmake ..
make
```

2. Set the environment variable in your shell profile:
```
# Add to ~/.bashrc or ~/.zshrc
export CLANG_TIDY_PLUGIN_PATH="/path/to/MagDragClangTidyChecks/build/lib/MagDragNamingCheck.so"
```

3. Copy the `.clang-tidy` configuration from the README.md to your repository root.

## IDE Integration

### Visual Studio Code
1. Install the "clang-tidy" extension
2. Add to settings.json:
```
{
    "clang-tidy.executable": "clang-tidy",
    "clang-tidy.checks": ["magdrag-naming-check"],
    "clang-tidy.compilerArgs": [],
    "clang-tidy.buildPath": "${workspaceFolder}/build"
}
```

### CLion
1. Enable clang-tidy under Settings → Editor → Inspections → C/C++ → Clang-Tidy
2. Add the environment variable to your run configurations
