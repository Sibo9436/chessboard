#include "renderer.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include <utility>

struct vertex {
  float x, y, z;
  float tx, ty;
  float texture = 0.0f;
};

ChessRenderer::ChessRenderer() : ChessRenderer(640, 480) {}
// This needs an absolute refactor cause I hate it with all my heart
ChessRenderer::ChessRenderer(unsigned int w, unsigned int h)
    : m_shader(Shader("../assets/shaders/test.vert.glsl",
                      "../assets/shaders/test.frag.glsl")),
      m_width(w), m_height(h) {
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  // VBO
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, 33 * 4 * sizeof(vertex), nullptr,
               GL_DYNAMIC_DRAW);
  // EBO
  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 33 * 6, nullptr,
               GL_DYNAMIC_DRAW);

  // VAO creation
  glGenBuffers(1, &m_vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (void *)(offsetof(vertex, tx)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (void *)(offsetof(vertex, texture)));
  glEnableVertexAttribArray(2);

  int samplers[2] = {0, 1};
  m_shader.bind();
  glUniform1iv(glGetUniformLocation(m_shader.m_id, "samplers"), 2, samplers);
}

inline float map(float x, float fromL, float fromU, float toL, float toU) {
  // così de botto senza senso
  if (fromU == fromL) {
    std::cerr << "You're trying to divide by zero and I hate you for this\n";

    return x;
  }
  return (x - fromL) / (fromU - fromL) * (toU - toL) - toU;
}

const inline float getAtlasX(ChessPiece piece) { return piece / 6.0f; }
const inline float getAtlasY(bool white) { return white ? 0.5f : 0.0f; }
void ChessRenderer::drawPiece(int x, int y, ChessPiece piece, bool white) {
  auto ortho = glm::ortho(0.0f, (float)m_width, 0.0f, (float)m_height);
  // Tutta sta roba poi sarà magicamente gestita dalla proiezione ortogonale :=
  int wstride = m_width / 8;
  int hstride = m_height / 8;
  x = (x * wstride);
  y = (y * hstride);
  float pixelx = map(x, 0, (float)m_width, -1.0f, 1.0f);
  float pixely = map(y, 0, (float)m_height, -1.0f, 1.0f);
  float ws = 2.0f * wstride / m_width;
  float hs = 2.0f * hstride / m_height;
  float ax = getAtlasX(piece);
  float ay = getAtlasY(white);
  //  std::cout << m_vertex_idx << ": adding piece at " << pixelx << " " <<
  //  pixely << "-" << ax << " " << ay << std::endl;
  vertex vertices[] = {
      vertex{pixelx, pixely, 0.0f, ax, ay},
      vertex{pixelx + ws, pixely, 0.0f, ax + 1.0f / 6.0f, ay},
      vertex{pixelx, pixely + hs, 0.0f, ax, ay + 0.5f},
      vertex{pixelx + ws, pixely + hs, 0.0f, ax + 1.0f / 6.0f, ay + 0.5f},
  };

  unsigned int start = m_vertex_idx;
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferSubData(GL_ARRAY_BUFFER, start * sizeof(vertex), 4 * sizeof(vertex),
                  vertices);

  unsigned int indices[] = {
      start + 0, start + 1, start + 2, start + 1, start + 2, start + 3,
  };
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_index_idx * sizeof(GLuint),
                  6 * sizeof(GLuint), indices);
  ;
  m_vertex_idx += 4;
  m_index_idx += 6;
}
void ChessRenderer::setShaderProgram(Shader shader) { m_shader = shader; }
void ChessRenderer::setTextureAtlas(const unsigned char *data, int width,
                                    int height, int nrChannels) {
  glGenTextures(1, &m_texture_atlas);
  m_shader.bind();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture_atlas);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  // m_shader.setInt("textureAtlas", 0);
}

void ChessRenderer::render() {
  m_shader.bind();
  glBindVertexArray(m_vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture_atlas);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_board_texture);
  glDrawElements(GL_TRIANGLES, m_index_idx, GL_UNSIGNED_INT, 0);
}
void ChessRenderer::clearRender() {
  render();
  m_vertex_idx = 4;
  m_index_idx = 6;
}

void ChessRenderer::boardTexture(unsigned char *data) {
  glGenTextures(1, &m_board_texture);
  m_shader.bind();
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_board_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE,
               data);
  glGenerateMipmap(GL_TEXTURE_2D);
}
void ChessRenderer::drawBoard(
    const std::vector<std::pair<int, int>> toHighlight) {
  unsigned char data[64 * 3];
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      int st = (x * 3) + y * 24;
      if (std::find(toHighlight.begin(), toHighlight.end(),
                    std::pair<int, int>(x, y)) != toHighlight.end()) {
        data[st + 0] = 0xff;
        data[st + 1] = 0xff;
        data[st + 2] = 0x00;
      } else if ((y + x) % 2 != 0) {
        data[st + 0] = 0xff;
        data[st + 1] = 0xff;
        data[st + 2] = 0xff;
      } else {
        data[st + 0] = 0x77;
        data[st + 1] = 0x77;
        data[st + 2] = 0x77;
      }
    }
  }
  boardTexture(data);
  vertex vertices[] = {
      vertex{-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
      vertex{1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
      vertex{-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
      vertex{1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
  };

  unsigned int start = m_vertex_idx;
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(vertex), vertices);

  unsigned int indices[] = {
      0, 1, 2, 1, 2, 3,
  };
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6 * sizeof(GLuint), indices);
  ;
}
