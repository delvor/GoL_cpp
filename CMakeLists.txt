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
    # set(SHARED_LIBS "/usr/lib")
endif()

# Finding SDL2 package
find_package(SDL2 REQUIRED)

# Fetch CFlags and libraries using bash and pkg-config
execute_process(
    COMMAND bash -c "sdl2-config --cflags"
    OUTPUT_VARIABLE SDL2_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
   COMMAND pkgconf --cflags SDL2_ttf
    OUTPUT_VARIABLE SDL2_OTHER_CFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND bash -c "sdl2-config --static-libs"
    OUTPUT_VARIABLE SDL2_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND pkgconf --libs --static SDL2_ttf
    OUTPUT_VARIABLE SDL2_OTHER_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "SDL2 Include Dirs: ${SDL2_CFLAGS}")
message(STATUS "SDL2 Libraries: ${SDL2_LIBS}")
message(STATUS "Other SDL2 include: ${SDL2_OTHER_CFLAGS}")
message(STATUS "Other SDL2 Libraries List: ${SDL2_OTHER_LIBS}")

# Source files
set(SOURCE_FILES src/main.cpp src/ui/Button.cpp src/game/gol.cpp )
set(HEADER_FILES src/ui/Button.h src/game/gol.h )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

# Create executable
add_executable(GameOfLife ${SOURCE_FILES} ${HEADER_FILES}  )

# Extract the include directories from SDL2_CFLAGS
string(REPLACE "-I" ";" SDL2_INCLUDE_DIRS ${SDL2_CFLAGS})

# Print the include directories for debug
message(STATUS "SDL2 Include Directories: ${SDL2_INCLUDE_DIRS}")

# Set the include directories for the target
target_include_directories(GameOfLife PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_OTHER_CFLAGS})

# Link the libraries, split by spaces if necessary
string(REPLACE " " ";" SDL2_LIBS_LIST ${SDL2_LIBS})
string(REPLACE " " ";" SDL2_OTHER_LIBS_LIST ${SDL2_OTHER_LIBS})

# Print the libraries for debugging
message(STATUS "SDL2 Libraries List: ${SDL2_LIBS}")
message(STATUS "Other SDL2 Libraries List: ${SDL2_OTHER_LIBS}")

# Properly split linker flags and ensure correct linking
separate_arguments(SDL2_LIBS_LIST UNIX_COMMAND "${SDL2_LIBS}")
separate_arguments(SDL2_OTHER_LIBS_LIST UNIX_COMMAND "${SDL2_OTHER_LIBS}")

# Link libraries
target_link_libraries(GameOfLife PRIVATE ${SDL2_LIBS_LIST} ${SDL2_OTHER_LIBS_LIST})

# Set linker flags to link statically
#set_target_properties(GameOfLife PROPERTIES
#    LINK_FLAGS "-mconsole")


# Disabling shared libraries
#set(BUILD_SHARED_LIBS OFF)
