cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(gbemu C)
set(CMAKE_MODULE_PATH "cmake")

###############################################################################
# Set build features
set(CMAKE_BUILD_TYPE Release)

###############################################################################
# Check headers
include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

set(INCLUDES "")
macro(ck_check_include_file header var)
  check_include_files("${INCLUDES};${header}" ${var})
  if(${var})
    set(INCLUDES ${INCLUDES} ${header})
  endif(${var})
endmacro(ck_check_include_file)

ck_check_include_file("stdlib.h" HAVE_STDLIB_H)

###############################################################################
# Check for integer types
check_type_size(intmax_t INTMAX_T)
check_type_size(uintmax_t UINTMAX_T)

check_type_size(pid_t PID_T)
if(NOT HAVE_PID_T)
  if(WIN32)
    set(pid_t "int")
  else(WIN32)
    MESSAGE(FATAL_ERROR "pid_t doesn't exist on this platform?")
  endif(WIN32)
endif(NOT HAVE_PID_T)

###############################################################################
# Platform-specific dependencies for SDL2
if(WIN32)
  set(SDL2_DIR "${PROJECT_SOURCE_DIR}/../windows_deps/sdl2")
  set(SDL2_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../windows_deps/sdl2/include;${PROJECT_SOURCE_DIR}/../windows_deps/sdl2/include/SDL2")

  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_LIBRARIES "${PROJECT_SOURCE_DIR}/../windows_deps/sdl2/lib/x64/SDL2.lib;${PROJECT_SOURCE_DIR}/../windows_deps/sdl2/lib/x64/SDL2main.lib")
  else ()
    set(SDL2_LIBRARIES "${PROJECT_SOURCE_DIR}/../windows_deps/sdl2/lib/x86/SDL2.lib;${PROJECT_SOURCE_DIR}/../windows_deps/sdl2/lib/x86/SDL2main.lib")
  endif ()
  string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

  set(SDL2_TTF_DIR "${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf")
  set(SDL2_TTF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf/include")

  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_TTF_LIBRARIES "${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf/lib/x64/SDL2_ttf.lib")
  else ()
    set(SDL2_TTF_LIBRARIES "${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf/lib/x86/SDL2_ttf.lib")
  endif ()
  string(STRIP "${SDL2_TTF_LIBRARIES}" SDL2_TTF_LIBRARIES)
else() # This block applies to macOS and other non-Windows systems
  # This section manually finds your local SDL2 frameworks to avoid the
  # problems with the pre-configured .cmake files you downloaded.

  find_library(SDL2_LIBRARY NAMES SDL2 HINTS "${PROJECT_SOURCE_DIR}/cmake/sdl2/SDL2.framework/Versions/A")
  find_path(SDL2_INCLUDE_DIR NAMES SDL.h HINTS "${PROJECT_SOURCE_DIR}/cmake/sdl2/SDL2.framework/Headers")

  find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf HINTS "${PROJECT_SOURCE_DIR}/cmake/sdl2/SDL2_ttf.framework/Versions/A")
  find_path(SDL2_TTF_INCLUDE_DIR NAMES SDL_ttf.h HINTS "${PROJECT_SOURCE_DIR}/cmake/sdl2/SDL2_ttf.framework/Headers")

  if(SDL2_LIBRARY AND SDL2_INCLUDE_DIR AND SDL2_TTF_LIBRARY AND SDL2_TTF_INCLUDE_DIR)
    set(SDL2_LIBRARIES "${SDL2_LIBRARY}")
    set(SDL2_INCLUDE_DIRS "${SDL2_INCLUDE_DIR}")
    set(SDL2_TTF_LIBRARIES "${SDL2_TTF_LIBRARY}")
    set(SDL2_TTF_INCLUDE_DIRS "${SDL2_TTF_INCLUDE_DIR}")
  else()
    message(FATAL_ERROR "Could not find local SDL2 frameworks or headers in 'cmake/sdl2'. Please check the path and file names.")
  endif()
endif()

###############################################################################
# Generate "config.h" from "cmake/config.h.cmake"
configure_file(cmake/config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DHAVE_CONFIG_H)
set(CONFIG_HEADER ${CMAKE_CURRENT_BINARY_DIR}/config.h)

###############################################################################
# Subdirectories
add_subdirectory(lib)
add_subdirectory(gbemu)