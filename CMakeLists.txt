# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(GameOfLife)

# Use sdl2-config to get flags for static linking
execute_process(
    COMMAND sdl2-config --cflags
    OUTPUT_VARIABLE SDL2_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND sdl2-config --static-libs
    OUTPUT_VARIABLE SDL2_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Set the source directory and the main source file
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(SOURCE_FILES "${SRC_DIR}/main.cpp")

# Add your executable
add_executable(GameOfLife ${SOURCE_FILES})

# Include SDL2 headers
target_include_directories(GameOfLife PRIVATE ${SDL2_CFLAGS})

# Link the static SDL2 library
# Split SDL2_LIBS into separate library flags
string(REPLACE " " ";" SDL2_LIBS_LIST ${SDL2_LIBS})
target_link_libraries(GameOfLife PRIVATE ${SDL2_LIBS_LIST})

# Ensure static linking and disable dynamic linking
set_target_properties(GameOfLife PROPERTIES
    LINK_SEARCH_END_STATIC TRUE
)

# Set flags for SDL2 static linking if necessary
target_compile_options(GameOfLife PRIVATE ${SDL2_CFLAGS})

# Explicitly add GCC static libraries for static linking
target_link_libraries(GameOfLife PRIVATE -static-libgcc -static-libstdc++)
