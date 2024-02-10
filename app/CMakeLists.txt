cmake_minimum_required(VERSION 3.20)

set (OPENGL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
add_executable(chessboard "main.cpp")
target_link_libraries(chessboard ${OPENGL_LIBRARIES} glfw glm glad imgui imgui_backend stb chess)
target_include_directories(chessboard PUBLIC  ../vendor/imgui ../vendor/imgui/backends ../vendor/glad ../vendor/stb ../src)
