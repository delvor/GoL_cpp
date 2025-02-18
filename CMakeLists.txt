cmake_minimum_required(VERSION 3.10)
project(GameOfLife)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_COMPILER "g++")


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
endif()


set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

if(WIN32)
    set(SDL "SDL2")  # Windows/MSYS2 uses SDL2
else()
    set(SDL "sdl2")  # Linux uses lowercase sdl2
endif()

execute_process(
    COMMAND pkgconf --cflags ${SDL} SDL2_ttf
    OUTPUT_VARIABLE SDL2_INCLUDE_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND pkgconf --libs --static ${SDL} SDL2_ttf
    OUTPUT_VARIABLE SDL2_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "SDL2 Include Dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 Libraries: ${SDL2_LIBS}")

# Source files
set(SOURCE_FILES src/main.cpp src/ui/Button.cpp)
set(HEADER_FILES src/ui/Button.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

add_executable(GameOfLife ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(GameOfLife PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(GameOfLife PRIVATE ${SDL2_LIBS} )

set_target_properties(GameOfLife PROPERTIES
    LINK_FLAGS "-static"
)
set(BUILD_SHARED_LIBS OFF)