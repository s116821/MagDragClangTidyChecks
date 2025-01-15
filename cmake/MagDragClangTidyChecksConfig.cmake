@PACKAGE_INIT@

set(MAGDRAG_VERSION @MAGDRAG_VERSION@)

# Find dependencies
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

# Set library paths
set(MAGDRAG_CLANGTIDY_LIBRARY_DIR "${CMAKE_CURRENT_LIST_DIR}/../lib")
set(MAGDRAG_CLANGTIDY_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../include")

# Export targets
include("${CMAKE_CURRENT_LIST_DIR}/MagDragClangTidyChecksTargets.cmake") 