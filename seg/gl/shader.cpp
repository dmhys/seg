#include "seg/gl/shader.h"

#include <stdexcept>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "seg/utilities/logger.h"

namespace {
const std::string general_vert =
#include "seg/gl/shader/shader.vert"
    ;
const std::string general_frag =
#include "seg/gl/shader/shader.frag"
    ;
const std::string grid_vert =
#include "seg/gl/shader/grid.vert"
    ;
const std::string grid_frag =
#include "seg/gl/shader/grid.frag"
    ;

struct ShaderSource {
  const std::string& vert;
  const std::string& frag;
};

ShaderSource getSource(seg::gl::ShaderType type) {
  switch (type) {
    case seg::gl::ShaderType::GRID:
      return {grid_vert, grid_frag};
    case seg::gl::ShaderType::GENERAL:
    default:
      return {general_vert, general_frag};
  }
}
}  // namespace

namespace seg {

namespace {
const char *modes[] = {"Uniform", "RGB", "RGBA", "Scalar", "Z-Axis"};
}

namespace gl {
void Shader::init(ShaderType type) {
  auto src = getSource(type);
  attatchShader(src.vert, src.frag);
  if (type == ShaderType::GENERAL) setDefaultSettings();
}

void Shader::attatchShader(const std::string& vert_src,
                           const std::string& frag_src) {
  // create shader & gl program
  GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
  GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  program = glCreateProgram();

  GLint result = GL_FALSE;
  int err_len;

  // compile vertex shader
  const char *vshader_cstr = vert_src.c_str();
  glShaderSource(vert_shader, 1, &vshader_cstr, nullptr);
  glCompileShader(vert_shader);
  glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &err_len);
    std::vector<char> err(err_len);
    glGetShaderInfoLog(vert_shader, err_len, nullptr, err.data());
    throw std::runtime_error("Error While Compiling Vertex Shader! " +
                             std::string(&err[0]));
  }

  // compile fragment shader
  const char *fshader_cstr = frag_src.c_str();
  glShaderSource(frag_shader, 1, &fshader_cstr, nullptr);
  glCompileShader(frag_shader);
  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &err_len);
    std::vector<char> err(err_len);
    glGetShaderInfoLog(frag_shader, err_len, nullptr, err.data());
    throw std::runtime_error("Error While Compiling Fragment Shader! " +
                             std::string(&err[0]));
  }

  // attatch & delete shaders
  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);
  glLinkProgram(program);
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  // error handle
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  if (result == GL_FALSE) {
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &err_len);
    std::vector<char> err(err_len);
    glGetProgramInfoLog(program, err_len, nullptr, err.data());
    throw std::runtime_error("Error While Attatching Shader! " +
                             std::string(&err[0]));
  }
}

void Shader::setDefaultSettings() {
  setUniform("visualize_hue_from", 1.0f);
  setUniform("visualize_hue_to", 0.3f);
  setUniform("visualize_saturation", 0.6f);
  setUniform("visualize_value", 0.7f);

  setUniform("visualize_z_min", 1.0f);
  setUniform("visualize_z_max", 0.0f);
}

void Shader::bind() { glUseProgram(program); }

void Shader::unbind() { glUseProgram(0); }

GLint Shader::getUniformId(const std::string &name) {
  auto cache = uniform_id_cache.find(name);
  if (cache != uniform_id_cache.end()) return cache->second;

  GLint id = glGetUniformLocation(program, name.c_str());

  if (id == -1)
    LOG_WARN("shader - GL uniform object [{}] can't be found.", name);
  else
    uniform_id_cache[name] = id;

  return id;
}

GLint Shader::getAttribId(const std::string &name) {
  auto cache = attrib_id_cache.find(name);
  if (cache != attrib_id_cache.end()) return cache->second;

  GLint id = glGetAttribLocation(program, name.c_str());

  if (id == -1)
    LOG_WARN("shader - GL Attrib object [{}] can't be found.", name);
  else
    attrib_id_cache[name] = id;

  return id;
}

}  // namespace gl

const char *enumToCharP(gl::ColorMode mode) {
  return modes[static_cast<int>(mode)];
}

}  // namespace seg