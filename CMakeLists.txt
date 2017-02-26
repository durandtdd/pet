cmake_minimum_required(VERSION 3.0)

project(pet CXX)


#==============================================================================#
# Executable sources
file(GLOB src_exe src/exe/*.cpp)
file(GLOB inc_exe src/exe/*.hpp)

# Core sources (dll and lib)
file(GLOB src_core src/core/*.cpp)
file(GLOB inc_core_p src/core/*.hpp)
file(GLOB_RECURSE inc_core include/*.hpp)
include_directories(include)

# Display in projects files
add_custom_target(include_exe SOURCES ${inc_exe})
add_custom_target(include_core_p SOURCES ${inc_core_p})
add_custom_target(include_core SOURCES ${inc_core})

# DLL
add_definitions(-DPET_DLL_EXPORT)
add_library(${PROJECT_NAME}dll SHARED ${src_core})

# LIB
add_definitions(-DPET_STATIC_LIBRARY)
add_library(${PROJECT_NAME}lib STATIC ${src_core})

# EXE
add_executable(${PROJECT_NAME} ${src_exe})
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}lib)


#==============================================================================#
# Create test files
add_library(test1 SHARED test/files/test1.cpp)
target_compile_definitions(test1 PRIVATE TEST_DLL_EXPORT)

add_library(test2 SHARED test/files/test2.cpp)
target_compile_definitions(test2 PRIVATE TEST_DLL_EXPORT)

add_executable(test3 test/files/test3.cpp)
target_link_libraries(test3 test2)

# Sources for test
file(GLOB src_test test/*.cpp)
file(GLOB inc_test test/*.hpp)

# Display in projects files
add_custom_target(include_test SOURCES ${inc_test})

# Create test exe
add_executable(tests ${src_test} ${PROJECT_SOURCE_DIR}/src/exe/tableprinter.cpp)
target_link_libraries(tests ${PROJECT_NAME}lib)
