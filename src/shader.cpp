#include "shader.hpp"
#include <fstream>
#include <iostream>

// directly stolen from...myself I guess?
GLuint compile_shader(const std::string &path, GLenum shader_type) {
  std::ifstream filestream(path);
  // posso fare di meglio direi
  std::string shader_src((std::istreambuf_iterator<char>(filestream)),
                         std::istreambuf_iterator<char>());
  GLuint shader_handle = glCreateShader(shader_type);
  const char *cstr = shader_src.c_str();
  glShaderSource(shader_handle, 1, &cstr, NULL);
  glCompileShader(shader_handle);

  int success;
  glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
  if (!success) {
    int info_size;
    glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &info_size);
    std::string compilation_info(info_size, ' ');
    glGetShaderInfoLog(shader_handle, info_size, NULL, compilation_info.data());
    std::cerr << "Error while compiling shader\n";
    std::cerr << compilation_info << std::endl;
    exit(EXIT_FAILURE);
  }
  return shader_handle;
}

GLuint create_shader_program(const std::string &vertex_path,
                             const std::string &fragment_path) {
  GLuint vertex_handle = compile_shader(vertex_path, GL_VERTEX_SHADER);
  GLuint fragment_handle = compile_shader(fragment_path, GL_FRAGMENT_SHADER);
  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_handle);
  glAttachShader(shader_program, fragment_handle);
  glLinkProgram(shader_program);
  int success;
  glGetProgramiv(shader_program, GL_COMPILE_STATUS, &success);
  if (!success) {
    int info_length = 0;
    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_length);
    std::string info_log(info_length, ' ');
    glGetProgramInfoLog(shader_program, info_length, NULL, info_log.data());
    std::cerr << "Error while linking shader program: [\n"
              << info_log << "\n]" << std::endl;
    std::cout << "Exiting" << std::endl;
    exit(EXIT_FAILURE);
  }
  glDetachShader(shader_program, vertex_handle);
  glDetachShader(shader_program, fragment_handle);
  glDeleteShader(vertex_handle);
  glDeleteShader(fragment_handle);
  return shader_program;
}

Shader::Shader(const std::string &vertex_path,
               const std::string &fragment_path) {
  m_id = create_shader_program(vertex_path, fragment_path);
}

void Shader::bind() { glUseProgram(m_id); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(m_id, name.c_str()), (GLint)value);
}
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(m_id, name.c_str()), (GLint)value);
}
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(m_id, name.c_str()), (GLfloat)value);
}
