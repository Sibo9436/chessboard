set (CHESS_SOURCES
  chess.hpp
  chess.cpp
  renderer.hpp
  renderer.cpp
  shader.cpp
  shader.hpp
  common.hpp
  gamestate.hpp
  gamestate.cpp
  )
add_library(chess STATIC ${CHESS_SOURCES})
target_link_libraries(chess ${OPENGL_LIBRARIES} glfw glm glad stb)
target_include_directories(chess PUBLIC ../vendor/glad ../vendor/stb)
