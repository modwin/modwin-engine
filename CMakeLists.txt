cmake_minimum_required(VERSION 3.28)
project(winther_engine)
set(CMAKE_CXX_STANDARD 17)

# CMake module path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

# Output directories
set(BASE_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIR}static")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIR}")

# Library paths
set(SDL3_DIR "C:/SDL3-3.1.6/x86_64-w64-mingw32/lib/cmake/SDL3")
set(spdlog_DIR "C:/spdlog-1.xx/build1")
set(SDL3_image_DIR "C:/SDL3_image-3.1.0/x86_64-w64-mingw32/lib/cmake/SDL3_image")
set(SDL3_image_INCLUDE_DIRS "C:/SDL3_image-3.1.0/x86_64-w64-mingw32/include")
set(TinyXML2_INCLUDE_DIR "C:/Program Files (x86)/tinyxml2/include")


# Querying CMake to find the required packages.
find_package(SDL3_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL3)
find_package(spdlog REQUIRED)

# Specifies the the IMGUI library.
add_library(IMGUI_LIB SHARED
        lib/imgui/imgui_impl_sdl3.h
        lib/imgui/imgui_impl_sdl3.cpp
        lib/imgui/imgui_impl_sdlrenderer3.cpp
        lib/imgui/imgui_impl_sdlrenderer3.h
        lib/imgui/imgui.h
        lib/imgui/imgui.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui/imstb_truetype.h
        lib/imgui/imstb_textedit.h
        lib/imgui/imstb_rectpack.h
        lib/imgui/imgui_internal.h
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_widgets.cpp
        include/physics/Position.h)

target_link_directories(IMGUI_LIB PUBLIC ${SDL3_INCLUDE_DIR})
target_include_directories(IMGUI_LIB PUBLIC ${SDL3_INCLUDE_DIR})

target_link_libraries(IMGUI_LIB PUBLIC
        SDL3::SDL3)


add_executable(WINTHER_SRC src/client.cpp)

# Specifies the include directories.
target_include_directories(WINTHER_SRC PUBLIC
        include
        lib/imgui
        ${TinyXML2_INCLUDE_DIR})


add_library(winther_engine SHARED
        src/client.cpp
        src/core/Engine.cpp
        src/core/Log.cpp
        src/graphics/TextureManager.cpp
        src/entity/EntityManager.cpp
        src/entity/Entity.cpp
        src/physics/Physics.cpp
        src/component/Component.cpp
        src/character/Player.cpp
        src/physics/Transform.cpp
        src/physics/Vector2D.cpp
        src/component/Animation.cpp
        include/component/Animation.h
        include/character/Character.h
        src/physics/RigidBody.cpp
        include/physics/RigidBody.h
        src/input/InputHandler.cpp
        include/input/InputHandler.h
        src/character/Character.cpp
        src/cfg/Config.cpp
        include/cfg/Config.h
        src/time/Time.cpp
        include/time/Time.h
        include/graphics/SurfaceLayer.h
        include/graphics/TileMap.h
        src/graphics/TileLayer.cpp
        include/graphics/TileLayer.h
        src/graphics/MapParser.cpp
        include/graphics/MapParser.h)

target_include_directories(winther_engine PUBLIC
        include
        lib/imgui
        ${TinyXML2_INCLUDE_DIR}
        )

add_subdirectory("C:/TINYXML2_SRC" tinyxml2_build)
link_directories("C:/Program Files (x86)/tinyxml2/lib/tinyxml2.lib")
link_directories("C:/Program Files (x86)/tinyxml2/lib/tinyxml2.dll")
link_directories("C:/Program Files (x86)/tinyxml2/lib/libtinyxml2.a")

target_link_libraries(winther_engine PUBLIC tinyxml2)
target_link_libraries(WINTHER_SRC PUBLIC tinyxml2)

#target_link_libraries(winther_engine PUBLIC ${TinyXML2_LIBRARIES})
#target_link_libraries(WINTHER_SRC PUBLIC ${TinyXML2_LIBRARIES})





target_link_libraries(winther_engine PUBLIC
        SDL3::SDL3
        spdlog::spdlog
        SDL3_image::SDL3_image-shared
        tinyxml2
        IMGUI_LIB)

target_link_libraries(WINTHER_SRC PUBLIC
        SDL3::SDL3
        spdlog::spdlog
        winther_engine
        SDL3_image::SDL3_image-shared
        tinyxml2
        IMGUI_LIB)





# Additional flags to ensure correct entry point
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-subsystem,console")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mconsole")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -e mainCRTStartup")