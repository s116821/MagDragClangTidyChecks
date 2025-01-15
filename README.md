# MagDrag Clang-Tidy Checks

Custom clang-tidy checks for enforcing MagDrag C++ coding standards.

## Requirements

- LLVM/Clang development packages
- CMake 3.13.4 or higher
- C++17 compiler

## Integration

1. Add the following to your project's CMakeLists.txt:

```cmake
include(FetchContent)

FetchContent_Declare(
  magdrag_checks
  URL https://github.com/yourusername/MagDragClangTidyChecks/releases/download/v1.0.0/magdrag-checks.tar.gz
)
FetchContent_MakeAvailable(magdrag_checks)
```

2. Create a `.clang-tidy` file in your project root with:
```yaml
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
```bash
clang-tidy your_file.cpp
```

## CI Integration

For GitLab CI, add this to your `.gitlab-ci.yml`:

```yaml
lint-clang_tidy-job:
  stage: Lint
  image: alpine:3.13
  before_script:
    - apk update
    - apk add --no-cache bash cmake clang clang-extra-tools build-base gcc g++ musl-dev
  script:
    - echo "Running clang-tidy checks..."
    - cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    # Older versions of clang-tidy (like in Alpine 3.13) don't support --config-file
    # Instead, we read the config file and pass its contents directly via --config
    - |
      CLANG_TIDY_CONFIG=$(cat ${CI_PROJECT_DIR}/.clang-tidy)
      clang-tidy -p build ${CI_PROJECT_DIR}/src/*.cpp --config="$CLANG_TIDY_CONFIG"
    - echo "Clang-tidy checks completed."
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

## Naming Conventions Enforced

### Variables
- Regular variables: `snake_case` (e.g., `player_score`, `current_state`)
- Class/struct member variables: `variable_name__O` (e.g., `player_data__O`, `game_state__O`)
- Container variables: `container_name__C` (e.g., `player_list__C`, `score_map__C`)
- Enum variables: `enum_name__E` (e.g., `player_state__E`, `game_mode__E`)
- Typedef variables: Resolved to either `__O` or `__C` based on underlying type
- Global variables: `UPPER_CASE` (e.g., `MAX_PLAYERS`, `DEFAULT_TIMEOUT`)
- Static const variables: `UPPER_CASE` (e.g., `STATIC_BUFFER_SIZE`)

### Functions & Methods
- Function names: `camelCase` (e.g., `calculateScore`, `updatePlayerState`)
- Method names: `camelCase` (e.g., `getValue`, `setPosition`)

### Types
- Class declarations: `PascalCase` (e.g., `PlayerManager`, `GameState`)
- Struct declarations: `PascalCase` (e.g., `PlayerData`, `ConfigSettings`)
- Enum declarations: `PascalCaseE` (e.g., `PlayerStateE`, `GameModeE`)
- Typedef declarations: `PascalCaseT` (e.g., `PlayerIdT`, `ScoreTypeT`)

### Other
- Macro definitions: `UPPER_CASE` (e.g., `DEBUG_MODE`, `MAX_BUFFER_SIZE`)
- Namespace names: `snake_case` (e.g., `game_logic`, `utils`)
- Template parameters: `PascalCase` (e.g., `template<typename DataType>`)

## Versioning

The package follows semantic versioning with automatic updates:

- Direct pushes to master: Major version bump (e.g., 1.0.0 → 2.0.0)
- PRs with 'feature' label: Minor version bump (e.g., 1.0.0 → 1.1.0)
- Other PRs and develop branch pushes: Patch version bump (e.g., 1.0.0 → 1.0.1)

New releases are automatically created on GitHub when changes are merged to master.
