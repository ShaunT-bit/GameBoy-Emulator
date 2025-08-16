set(MAIN_SOURCES
        main.c
)

file (GLOB headers "${PROJECT_SOURCE_DIR}/include/*.h")

add_executable(gbemu ${HEADERS} ${MAIN_SOURCES})
target_link_libraries(gbemu PRIVATE "${SDL2_LIBRARIES}" "${SDL2_TTF_LIBRARIES}" emu)
target_include_directories(gbemu PUBLIC ${PROJECT_SOURCE_DIR}/include )


install(TARGETS gbemu
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

add_custom_command(
        TARGET gbemu POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/NotoSansMono-Medium.ttf
        $<TARGET_FILE_DIR:gbemu>
        VERBATIM)

if (APPLE)
  # Set the rpath so the executable looks for frameworks in its own directory.
  set_target_properties(gbemu PROPERTIES
          INSTALL_NAME_DIR "@rpath"
          BUILD_WITH_INSTALL_RPATH ON
          MACOSX_RPATH ON
          LINK_FLAGS "-Wl,-rpath,@executable_path"
  )

  # Copy the SDL2 framework to the executable's directory.
  add_custom_command(
          TARGET gbemu POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${PROJECT_SOURCE_DIR}/cmake/sdl2/SDL2.framework"
          "$<TARGET_FILE_DIR:gbemu>/SDL2.framework"
          VERBATIM
  )

  # Copy the SDL2_ttf framework to the executable's directory.
  add_custom_command(
          TARGET gbemu POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${PROJECT_SOURCE_DIR}/cmake/sdl2/SDL2_ttf.framework"
          "$<TARGET_FILE_DIR:gbemu>/SDL2_ttf.framework"
          VERBATIM
  )
endif()

if (WIN32)
  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    add_custom_command(
            TARGET gbemu POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/../windows_deps/sdl2/lib/x64/SDL2.dll
            $<TARGET_FILE_DIR:gbemu>
            VERBATIM)
    add_custom_command(
            TARGET gbemu POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf/lib/x64/SDL2_ttf.dll
            $<TARGET_FILE_DIR:gbemu>
            VERBATIM)
    add_custom_command(
            TARGET gbemu POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf/lib/x64/zlib1.dll
            $<TARGET_FILE_DIR:gbemu>
            VERBATIM)
    add_custom_command(
            TARGET gbemu POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf/lib/x64/libfreetype-6.dll
            $<TARGET_FILE_DIR:gbemu>
            VERBATIM)
  else ()
    add_custom_command(
            TARGET gbemu POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/../windows_deps/sdl2/lib/x86/SDL2.dll
            $<TARGET_FILE_DIR:gbemu>
            VERBATIM)
    add_custom_command(
            TARGET gbemu POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf/lib/x86/SDL2_ttf.dll
            $<TARGET_FILE_DIR:gbemu>
            VERBATIM)
    add_custom_command(
            TARGET gbemu POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf/lib/x86/zlib1.dll
            $<TARGET_FILE_DIR:gbemu>
            VERBATIM)
    add_custom_command(
            TARGET gbemu POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf/lib/x86/libfreetype-6.dll
            $<TARGET_FILE_DIR:gbemu>
            VERBATIM)
  endif ()
endif()