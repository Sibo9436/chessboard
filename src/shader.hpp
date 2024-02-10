#ifndef INCLUDE_SRC_SHADER_HPP_
#define INCLUDE_SRC_SHADER_HPP_
#include <glad/glad.h>
#include <string>

class Shader {
public:
  Shader(const std::string &vertex_path, const std::string &fragment_path);
  void bind();
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;

  GLuint m_id;
};

#endif // INCLUDE_SRC_SHADER_HPP_
