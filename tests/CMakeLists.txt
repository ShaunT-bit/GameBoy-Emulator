include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CHECK REQUIRED check)
include_directories(${CHECK_INCLUDE_DIRS})
include_directories("/usr/local/include")
link_directories(${CHECK_LIBRARY_DIRS})

set(TEST_SOURCES
        check_gbe.c
)

add_executable(check_gbe ${TEST_SOURCES})
target_link_libraries(check_gbe emu ${CHECK_LIBRARIES})
target_include_directories(check_gbe PRIVATE ../include)


find_program(DEBIAN "dpkg")
if(DEBIAN)
    target_link_libraries(check_gbe check m rt pthread subunit)
endif()

if (WIN32)
target_include_directories(emu PUBLIC ${PROJECT_SOURCE_DIR}/../windows_deps/check )
endif()
