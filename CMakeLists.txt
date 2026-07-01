cmake_minimum_required(VERSION 3.10)
project(HiraethEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenGL_GL_PREFERENCE "GLVND")

# --- Dependencies ---
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG QUIET)
find_package(assimp REQUIRED)

# If not found (Windows), download and build it automatically
if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found on system. Downloading from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw
            GIT_TAG        3.4
    )

    # prevent GLFW from building its own examples/tests/docs to save time
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(glfw)
endif()



# --- Sources ---
add_executable(HiraethEngine
        src/main.cpp
        src/glad.c

        # ImGui Core
        lib/imgui/imgui.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_widgets.cpp

        # ImGui Backends
        lib/imgui/imgui_impl_glfw.cpp
        lib/imgui/imgui_impl_opengl3.cpp

        # Your Project Files
        src/Shapes.cpp   include/Shapes.h
        src/View.cpp     include/View.h
        src/Shaders.cpp  include/Shaders.h
                         include/Camera.h
        src/Scene.cpp    include/Scene.h
        src/Menu.cpp     include/Menu.h
                         include/SceneParameters.h
                         include/utils.h
        src/Texture.cpp  include/Texture.h
        src/Material.cpp include/Material.h
        src/Mesh.cpp     include/Mesh.h
        src/Model.cpp    include/Model.h
        src/ModelLibrary.cpp
        include/ModelLibrary.h
        include/json.hpp
)

set_source_files_properties(src/glad.c PROPERTIES LANGUAGE C)

# --- Includes ---
target_include_directories(HiraethEngine PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/lib/imgui  # Updated to match your source folder "lib"
        ${CMAKE_SOURCE_DIR}/textures
)

if (WIN32)
    target_include_directories(HiraethEngine PRIVATE "C:/libraries/glm")
endif()

# --- Linking (General) ---
if(TARGET OpenGL::OpenGL)
    target_link_libraries(HiraethEngine PRIVATE OpenGL::OpenGL)
elseif(TARGET OpenGL::GL)
    target_link_libraries(HiraethEngine PRIVATE OpenGL::GL)
else()
    message(FATAL_ERROR "No suitable OpenGL target found")
endif()

target_link_libraries(HiraethEngine PRIVATE glfw assimp::assimp)

# --- OS Specific Linking (The Key Part!) ---
if(APPLE)
    # macOS Specifics
    target_link_libraries(HiraethEngine PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
elseif(WIN32)
    # Windows Specifics
    # Windows doesn't need 'dl' or 'pthread'.
    # If using static GLFW, you might sometimes need these, but usually 'glfw' handles it:
    # target_link_libraries(HiraethEngine PRIVATE gdi32 user32 shell32)
elseif(UNIX)
    # Linux Specifics (The 'else' block you had before)
    # FIX: Changed target name from 'opengl' (typo) to 'HiraethEngine'
    target_link_libraries(HiraethEngine PRIVATE
            dl
            pthread
    )
endif()