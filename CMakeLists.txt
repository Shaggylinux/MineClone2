cmake_minimum_required(VERSION 3.15)
project(MineClone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# --- GLM (headers only, solo incluye la carpeta) ---
add_subdirectory(external/glm)
# --- REQUERIR OPENGL
find_package(OpenGL REQUIRED)

# --- GLFW ---
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# --- Ejecutable principal ---
add_executable(MineClone2 src/main.cpp)

# --- Enlazar dependencias ---
target_link_libraries(MineClone2 PRIVATE
    glfw
    OpenGL::GL
)

# --- Incluir rutas ---
target_include_directories(MineClone2 PRIVATE
    external/glm
)
