# BinaryCodeGenMat

A C++20 library representing a binary generator matrix whose rows are `BinaryCodeWord`s.

## Build

```bash
cmake -S . -B build 
cmake --build build
```

## Examples

```bash
cmake -S . -B build  -DBCGM_BUILD_EXAMPLES=ON
cmake --build build
./build/examples/bcgm_example_basic
```

## Tests

```bash
cmake -S . -B build  -DBCGM_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build
```

## Consume via FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
  BinaryCodeGenMat
  GIT_REPOSITORY https://github.com/<you>/BinaryCodeGenMat.git
  GIT_TAG v0.1.0
)

set(BCW_GIT_REPOSITORY https://github.com/<you>/BinaryCodeWord.git CACHE STRING "" FORCE)
set(BCW_GIT_TAG v1.1.0 CACHE STRING "" FORCE)

set(BCGM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BCGM_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(BinaryCodeGenMat)

target_link_libraries(your_target PRIVATE BinaryCodeGenMat::BinaryCodeGenMat)
```
