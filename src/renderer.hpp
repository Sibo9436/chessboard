#ifndef INCLUDE_CHESS_RENDERER_HPP_
#define INCLUDE_CHESS_RENDERER_HPP_
#include "common.hpp"
#include "shader.hpp"
#include <glad/glad.h>
#include <vector>
class ChessRenderer {
private:
  GLuint m_vbo;
  GLuint m_ebo;
  GLuint m_vao;
  Shader m_shader;
  GLuint m_texture_atlas;
  GLuint m_board_texture = 0;
  size_t m_index_idx = 6;
  size_t m_vertex_idx = 4;
  unsigned int m_width, m_height;

public:
  ChessRenderer();
  ChessRenderer(unsigned int width, unsigned int height);
  void setShaderProgram(Shader shader);
  void setTextureAtlas(const unsigned char *data, int width, int height,
                       int channels);
  void render();
  void clearRender();
  // remember that you're not writing a fucking library
  void drawPiece(int x, int y, ChessPiece piece, bool white);
  void drawBoard(const std::vector<std::pair<int, int>> toHighlight);

private:
  void boardTexture(unsigned char *data);
};
#endif // INCLUDE_CHESS_RENDERER_HPP_
