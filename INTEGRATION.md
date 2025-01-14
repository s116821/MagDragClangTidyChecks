# Integration Guide

## Adding to Your Project

1. Add the following to your project's CMakeLists.txt:

```cmake
include(FetchContent)

FetchContent_Declare(
  magdrag_checks
  URL https://github.com/yourusername/MagDragClangTidyChecks/releases/download/v1.0.0/magdrag-checks.tar.gz
)
FetchContent_MakeAvailable(magdrag_checks)
```

2. Create a `.clang-tidy` file in your project root (see configuration in README.md)

3. Run clang-tidy:
```bash
clang-tidy your_file.cpp
```

## CI Integration

For GitLab CI, add this to your `.gitlab-ci.yml`:

```yaml
clang_tidy:
  stage: lint
  image: ubuntu:latest
  before_script:
    - apt-get update && apt-get install -y cmake llvm-dev libclang-dev clang clang-tidy
  script:
    - cmake -B build
    - cmake --build build
    - find . -name "*.cpp" -o -name "*.hpp" | xargs clang-tidy
```

For GitHub Actions, add this to your workflow:

```yaml
lint:
  runs-on: ubuntu-latest
  steps:
    - uses: actions/checkout@v3
    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install -y cmake llvm-dev libclang-dev clang clang-tidy
    - name: Build and run clang-tidy
      run: |
        cmake -B build
        cmake --build build
        find . -name "*.cpp" -o -name "*.hpp" | xargs clang-tidy
```
