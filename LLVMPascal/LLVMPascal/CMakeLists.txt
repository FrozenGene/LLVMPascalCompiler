cmake_minimum_required(VERSION 3.1.3)

project("LLVMPascalCompiler")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")
endif()

set(SOURCE_FILES ast.h ast.cpp constant.h constant.cpp dictionary.h dictionary.cpp
                 error.h error.cpp main.cpp parser.h parser.cpp scanner.h scanner.cpp
                 token.h token.cpp)

add_executable(lpc ${SOURCE_FILES})

# Tempory for program test. So copy test file to build directory
file(GLOB PASCAL_TEST_FILES "*.pas")
file(COPY ${PASCAL_TEST_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
