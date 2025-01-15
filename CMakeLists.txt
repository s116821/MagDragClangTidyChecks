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
add_library(MagDragClangTidyNamingCheck MODULE
  src/magDragNamingCheck.cpp
)

# Add include directories for the plugin
target_include_directories(MagDragClangTidyNamingCheck PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/inc
  ${LLVM_INCLUDE_DIRS}
  ${CLANG_INCLUDE_DIRS}
)

# Set output properties for the library
if(UNIX)
  set_target_properties(MagDragClangTidyNamingCheck
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PREFIX ""
    OUTPUT_NAME "MagDragClangTidyNamingCheck"
  )
elseif(WIN32)
  set_target_properties(MagDragClangTidyNamingCheck
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PREFIX ""
    SUFFIX ".dll"
    OUTPUT_NAME "MagDragClangTidyNamingCheck"
  )
endif()

# Add Windows-specific compile definitions
if(WIN32)
  target_compile_definitions(MagDragClangTidyNamingCheck PRIVATE
    _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
    _CRT_SECURE_NO_WARNINGS
  )
endif()

# Link against required LLVM/Clang libraries
target_link_libraries(MagDragClangTidyNamingCheck
  PRIVATE
  clangTidy
  clangTidyUtils
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  LLVMSupport
)

# Version information
set(MAGDRAG_VERSION_MAJOR 1)
set(MAGDRAG_VERSION_MINOR 0)
set(MAGDRAG_VERSION_PATCH 1)
set(MAGDRAG_VERSION "${MAGDRAG_VERSION_MAJOR}.${MAGDRAG_VERSION_MINOR}.${MAGDRAG_VERSION_PATCH}")

# Configure version header
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/Version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/Version.h"
)

include(CMakePackageConfigHelpers)

# Install targets
install(TARGETS MagDragClangTidyNamingCheck
  EXPORT MagDragClangTidyChecksTargets
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(DIRECTORY include/
  DESTINATION include
)

# Generate and install package files
configure_package_config_file(
  cmake/MagDragClangTidyChecksConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/MagDragClangTidyChecksConfig.cmake
  INSTALL_DESTINATION lib/cmake/MagDragClangTidyChecks
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/MagDragClangTidyChecksConfigVersion.cmake
  VERSION ${MAGDRAG_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/MagDragClangTidyChecksConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/MagDragClangTidyChecksConfigVersion.cmake
  DESTINATION lib/cmake/MagDragClangTidyChecks
)

install(EXPORT MagDragClangTidyChecksTargets
  FILE MagDragClangTidyChecksTargets.cmake
  DESTINATION lib/cmake/MagDragClangTidyChecks
) 