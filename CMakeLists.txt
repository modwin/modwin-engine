cmake_minimum_required(VERSION 3.28)

project(modwin_engine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

# Keep generated files in the build tree instead of mixing them with source files.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Local dependency locations. Override these cache variables from CLion or with
# -D<variable>=<path> when dependencies are installed elsewhere.
set(SDL3_DIR "C:/SDL3-3.1.6/x86_64-w64-mingw32/lib/cmake/SDL3" CACHE PATH "SDL3 CMake package directory")
set(SDL3_image_DIR "C:/SDL3_image-3.1.0/x86_64-w64-mingw32/lib/cmake/SDL3_image" CACHE PATH "SDL3_image CMake package directory")
set(spdlog_DIR "C:/spdlog-1.xx/build1" CACHE PATH "spdlog CMake package directory")
set(MODWIN_TINYXML2_SOURCE_DIR "C:/TINYXML2_SRC" CACHE PATH "TinyXML2 source directory")

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

if(EXISTS "${MODWIN_TINYXML2_SOURCE_DIR}/CMakeLists.txt")
    set(tinyxml2_BUILD_TESTING OFF CACHE BOOL "Build TinyXML2's own tests" FORCE)
    add_subdirectory(
        "${MODWIN_TINYXML2_SOURCE_DIR}"
        "${CMAKE_BINARY_DIR}/external/tinyxml2"
        EXCLUDE_FROM_ALL
    )
else()
    find_package(tinyxml2 CONFIG REQUIRED)
endif()

add_library(modwin_imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_impl_sdl3.cpp
    external/imgui/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(modwin_imgui PUBLIC external/imgui)
target_link_libraries(modwin_imgui PUBLIC SDL3::SDL3)

add_library(modwin_engine STATIC
    src/cfg/Config.cpp
    src/character/Character.cpp
    src/character/Player.cpp
    src/component/Animation.cpp
    src/component/Component.cpp
    src/core/Engine.cpp
    src/core/Log.cpp
    src/core/ResourcePaths.cpp
    src/entity/Entity.cpp
    src/entity/EntityManager.cpp
    src/graphics/MapDocument.cpp
    src/graphics/MapParser.cpp
    src/graphics/MapSerializer.cpp
    src/graphics/TextureManager.cpp
    src/graphics/TileCoordinate.cpp
    src/graphics/TileLayer.cpp
    src/input/InputHandler.cpp
    src/physics/Physics.cpp
    src/physics/RigidBody.cpp
    src/physics/Transform.cpp
    src/physics/Vector2D.cpp
    src/time/Time.cpp
    src/editor/EditorState.cpp
    src/editor/EditorUI.cpp
    src/editor/EditorCamera.cpp
    src/editor/EditorController.cpp
)

add_library(modwin::engine ALIAS modwin_engine)

target_include_directories(modwin_engine PUBLIC include)

target_link_libraries(modwin_engine
    PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image-shared
        spdlog::spdlog
        tinyxml2::tinyxml2
        modwin_imgui
)

add_executable(modwin_game src/main.cpp)
target_link_libraries(modwin_game PRIVATE modwin::engine)

if(BUILD_TESTING)
    add_executable(modwin_tests tests/TileEditingTests.cpp)
    target_link_libraries(modwin_tests PRIVATE modwin::engine)

    add_test(
        NAME modwin_tile_editing_tests
        COMMAND modwin_tests
            "${CMAKE_SOURCE_DIR}/resources/maps/map.tmx"
            "${CMAKE_BINARY_DIR}/test-output"
    )

    get_filename_component(MODWIN_COMPILER_DIRECTORY "${CMAKE_CXX_COMPILER}" DIRECTORY)
    set_tests_properties(modwin_tile_editing_tests PROPERTIES
        ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${MODWIN_COMPILER_DIRECTORY}"
    )
endif()

if(MSVC)
    target_compile_options(modwin_engine PRIVATE /W4)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(modwin_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Place runtime libraries beside the executable for local development.
if(WIN32)
    add_custom_command(TARGET modwin_game POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE_DIR:modwin_game>"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:SDL3_image::SDL3_image-shared>"
            "$<TARGET_FILE_DIR:modwin_game>"
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:modwin_game>/resources"
        VERBATIM
    )
endif()

if(MINGW)
    get_filename_component(MODWIN_MINGW_RUNTIME_DIRECTORY "${CMAKE_CXX_COMPILER}" DIRECTORY)
    foreach(MODWIN_RUNTIME_DLL libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll)
        if(EXISTS "${MODWIN_MINGW_RUNTIME_DIRECTORY}/${MODWIN_RUNTIME_DLL}")
            add_custom_command(TARGET modwin_game POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${MODWIN_MINGW_RUNTIME_DIRECTORY}/${MODWIN_RUNTIME_DLL}"
                    "$<TARGET_FILE_DIR:modwin_game>"
                VERBATIM
            )
        endif()
    endforeach()
endif()
