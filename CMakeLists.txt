cmake_minimum_required(VERSION 3.13.4)
project(MagDragClangTidyChecks)

# Find LLVM and Clang packages
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

find_package(Clang REQUIRED CONFIG)
message(STATUS "Using ClangConfig.cmake in: ${Clang_DIR}")

# Set include directories
include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

# Set additional compiler flags
add_definitions(${LLVM_DEFINITIONS})
set(CMAKE_CXX_STANDARD 17)

# Create the clang-tidy plugin library
add_library(MagDragNamingCheck MODULE
  src/magDragNamingCheck.cpp
)

# Add include directories for the plugin
target_include_directories(MagDragNamingCheck PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/inc
  ${LLVM_INCLUDE_DIRS}
  ${CLANG_INCLUDE_DIRS}
)

# Set output properties for the library
if(UNIX)
  set_target_properties(MagDragNamingCheck
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PREFIX ""
    OUTPUT_NAME "MagDragNamingCheck"  # Ensure consistent naming
  )
endif()

# Link against required LLVM/Clang libraries
target_link_libraries(MagDragNamingCheck
  PRIVATE
  clangTidy
  clangTidyUtils
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  LLVMSupport
) 