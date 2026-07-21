cmake_minimum_required(VERSION 3.24)

cmake_minimum_required(VERSION 3.24)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING ""
    )
endif()


project(handheld_ui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

foreach(CONFIG Debug Release RelWithDebInfo MinSizeRel)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG}
        ${CMAKE_SOURCE_DIR}/bin)
endforeach()


set(COMMON_SOURCES
    src/root/app.cpp

    src/display/framebuffer.cpp
    src/display/renderer.cpp
    src/display/bitmap.cpp
    src/display/fonts.cpp
)


# Desktop SDL version
find_package(SDL3 CONFIG REQUIRED)

add_executable(handheld_ui_desktop
    ${COMMON_SOURCES}
    src/main_desktop.cpp
    src/display/sdl3_display.cpp
)

target_include_directories(handheld_ui_desktop PRIVATE
    include
    include/root
    include/display
)

target_link_libraries(handheld_ui_desktop PRIVATE
    SDL3::SDL3
)


# RK3566 version
add_executable(handheld_ui_rk3566
    ${COMMON_SOURCES}
    src/main_production.cpp
    src/display/uc8179_display.cpp
)

target_include_directories(handheld_ui_rk3566 PRIVATE
    include
    include/root
    include/display
)