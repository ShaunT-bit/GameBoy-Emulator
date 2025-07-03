cmake_minimum_required(VERSION 3.10)
project(gameboy C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

if (APPLE)
    # Determine Homebrew paths based on architecture
    set(HOMEBREW_PREFIX "")
    if (EXISTS "/opt/homebrew") # Apple Silicon
        set(HOMEBREW_PREFIX "/opt/homebrew")
    elseif (EXISTS "/usr/local") # Intel
        set(HOMEBREW_PREFIX "/usr/local")
    endif()

    if (NOT HOMEBREW_PREFIX STREQUAL "")
        # Add Homebrew's paths to CMake's search paths for packages, libraries, and includes
        list(APPEND CMAKE_PREFIX_PATH ${HOMEBREW_PREFIX})
        list(APPEND CMAKE_LIBRARY_PATH ${HOMEBREW_PREFIX}/lib)
        # Point CMAKE_INCLUDE_PATH to the *base* include directory
        list(APPEND CMAKE_INCLUDE_PATH ${HOMEBREW_PREFIX}/include)
    else()
        message(WARNING "Homebrew installation not found at /opt/homebrew or /usr/local. SDL2 linking might fail.")
    endif()
endif()

# Find SDL2.
# We will NOT use COMPONENTS SDL2 or CONFIG here.
# Instead, we will search for the specific library file.
find_library(SDL2_LIBRARY
        NAMES SDL2
        PATHS
        ${HOMEBREW_PREFIX}/lib # Explicitly search Homebrew's lib directory first
        NO_SYSTEM_ENVIRONMENT_PATH # Do not use env vars
        NO_CMAKE_SYSTEM_PATH       # Do not use system paths (crucial to avoid /Library/Frameworks)
        REQUIRED
)

find_path(SDL2_INCLUDE_DIR
        NAMES SDL.h
        PATHS
        ${HOMEBREW_PREFIX}/include/SDL2 # Explicitly search Homebrew's include directory for SDL.h inside SDL2/
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH
        REQUIRED
)

# If SDL2_LIBRARY and SDL2_INCLUDE_DIR are found, define an imported target
if (SDL2_LIBRARY AND SDL2_INCLUDE_DIR)
    add_library(SDL2::SDL2 UNKNOWN IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
            IMPORTED_LOCATION "${SDL2_LIBRARY}"
            # Set the interface include directory to the *parent* of SDL2/
            # so that #include <SDL2/SDL.h> works correctly.
            INTERFACE_INCLUDE_DIRECTORIES "${HOMEBREW_PREFIX}/include"
            IMPORTED_LINK_INTERFACE_RPATH "${HOMEBREW_PREFIX}/lib"
    )
else()
    message(FATAL_ERROR "SDL2 library or include directory not found. Please ensure SDL2 is installed via Homebrew.")
endif()


add_executable(gameboy main.c cpu.c)

# Add the *base* include directory for SDL2
# This makes sure the compiler finds /opt/homebrew/include, so #include <SDL2/SDL.h> works.
target_include_directories(gameboy PRIVATE "${HOMEBREW_PREFIX}/include")


# Link your executable to the SDL2 library using the imported target
target_link_libraries(gameboy PRIVATE SDL2::SDL2)

# --- Ensure RPATH is set for the executable (double-check) ---
if (APPLE AND TARGET gameboy)
    set_property(TARGET gameboy APPEND PROPERTY
            BUILD_RPATH "${HOMEBREW_PREFIX}/lib"
            INSTALL_RPATH "${HOMEBREW_PREFIX}/lib"
    )
    set_property(TARGET gameboy APPEND PROPERTY
            BUILD_RPATH "@loader_path/../lib"
            INSTALL_RPATH "@loader_path/../lib"
    )
endif()