cmake_minimum_required(VERSION 3.10)
project(gameboy C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# ---- macOS Homebrew Setup ----
if (APPLE)
    # ✅ MUST be set before any find_package/find_library calls
    set(CMAKE_FIND_FRAMEWORK NEVER)

    set(HOMEBREW_PREFIX "")
    if (EXISTS "/opt/homebrew")
        set(HOMEBREW_PREFIX "/opt/homebrew")
    elseif (EXISTS "/usr/local")
        set(HOMEBREW_PREFIX "/usr/local")
    endif()

    if (NOT HOMEBREW_PREFIX STREQUAL "")
        list(APPEND CMAKE_PREFIX_PATH ${HOMEBREW_PREFIX})
        list(APPEND CMAKE_LIBRARY_PATH ${HOMEBREW_PREFIX}/lib)
        list(APPEND CMAKE_INCLUDE_PATH ${HOMEBREW_PREFIX}/include)
    else()
        message(WARNING "Homebrew not found.")
    endif()
endif()

# ---- Find SDL2 ----
find_library(SDL2_LIBRARY
        NAMES SDL2
        PATHS ${HOMEBREW_PREFIX}/lib
        NO_DEFAULT_PATH
        REQUIRED
)

find_path(SDL2_INCLUDE_DIR
        NAMES SDL.h
        PATHS ${HOMEBREW_PREFIX}/include/SDL2
        NO_DEFAULT_PATH
        REQUIRED
)

# ---- Find SDL2_ttf ----
find_library(SDL2_TTF_LIBRARY
        NAMES SDL2_ttf
        PATHS ${HOMEBREW_PREFIX}/lib
        NO_DEFAULT_PATH
        REQUIRED
)

# ---- Debug output ----
message(STATUS "SDL2_LIBRARY: ${SDL2_LIBRARY}")
message(STATUS "SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")
message(STATUS "SDL2_TTF_LIBRARY: ${SDL2_TTF_LIBRARY}")

# ---- Collect Sources ----
file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.c"
)

# ---- Define Executable ----
add_executable(gameboy ${SOURCES})

target_include_directories(gameboy PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${SDL2_INCLUDE_DIR}"
        "${HOMEBREW_PREFIX}/include"
)

target_link_libraries(gameboy PRIVATE
        "${SDL2_LIBRARY}"
        "${SDL2_TTF_LIBRARY}"
)

# ---- Set RPATH for macOS ----
if (APPLE)
    set_target_properties(gameboy PROPERTIES
            BUILD_RPATH "${HOMEBREW_PREFIX}/lib"
            INSTALL_RPATH "${HOMEBREW_PREFIX}/lib"
    )
endif()