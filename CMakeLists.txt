cmake_minimum_required(VERSION 3.10)


project(GameOfLife)

# Fetch SDL2 flags and libraries
execute_process(
    COMMAND bash -c "sdl2-config --cflags"
    OUTPUT_VARIABLE SDL2_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

#execute_process(
#    COMMAND bash -c "sdl2-config --static-libs"
#    OUTPUT_VARIABLE SDL2_LIBS
#    OUTPUT_STRIP_TRAILING_WHITESPACE
#)

execute_process(
    COMMAND bash -c "sdl2-config --static-libs"
    OUTPUT_VARIABLE SDL2_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "SDL2_cflags: ${SDL2_CFLAGS}")
message(STATUS "SDL2_LIBS: ${SDL2_LIBS}")
# Split libraries into a list
string(REPLACE " " ";" SDL2_LIBS_LIST ${SDL2_LIBS})


# Set source files
set(SOURCE_FILES src/main.cpp)

# Create executable
add_executable(GameOfLife ${SOURCE_FILES})

# Include SDL2 headers
target_include_directories(GameOfLife PRIVATE ${SDL2_CFLAGS})

# Link SDL2main and other SDL2 libraries
target_link_libraries(GameOfLife PRIVATE ${SDL2_LIBS_LIST})

# Add flags for static linking if required
target_compile_options(GameOfLife PRIVATE -static)

