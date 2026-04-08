#pragma once

#include <string>
#include <unordered_map>

#include <Eigen/Dense>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace seg {
namespace gl {

enum class ShaderType { GENERAL, GRID };

enum class ColorMode {
  UNIFORM = 0,
  RGB = 1,
  RGBA = 2,
  SCALAR = 3,
  ZAXIS = 4,
};

class Shader {
 public:
  Shader() {};
  ~Shader();
  void init(ShaderType type);

  void bind();
  void unbind();

  ShaderType getType() const { return type; }
  GLint getUniformId(const std::string& name);
  GLint getAttribId(const std::string& name);

  inline void setModelMatrix(const Eigen::Matrix4f& model_matrix) {
    setUniform("model_matrix", model_matrix);
  }

  inline void setColorMode(const ColorMode mode) {
    setUniform("color_mode", static_cast<int>(mode));
  }

  inline void setUniform(const std::string& name, int value) {
    glUniform1i(getUniformId(name), value);
  }
  inline void setUniform(const std::string& name, float value) {
    glUniform1f(getUniformId(name), value);
  }
  inline void setUniform(const std::string& name,
                         const Eigen::Vector2f& value) {
    glUniform2fv(getUniformId(name), 1, value.data());
  }
  inline void setUniform(const std::string& name,
                         const Eigen::Vector3f& value) {
    glUniform3fv(getUniformId(name), 1, value.data());
  }
  inline void setUniform(const std::string& name,
                         const Eigen::Vector4f& value) {
    glUniform4fv(getUniformId(name), 1, value.data());
  }
  inline void setUniform(const std::string& name,
                         const Eigen::Matrix4f& value) {
    glUniformMatrix4fv(getUniformId(name), 1, GL_FALSE, value.data());
  }

 private:
  void attatchShader(const std::string& vert_src, const std::string& frag_src);
  void setDefaultSettings();

  ShaderType type = ShaderType::GENERAL;
  GLuint program = 0;
  std::unordered_map<std::string, GLint> uniform_id_cache;
  std::unordered_map<std::string, GLint> attrib_id_cache;

};  // class Shader
}  // namespace gl

const char* enumToCharP(gl::ColorMode mode);

}  // namespace seg
