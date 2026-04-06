#include "seg/gl/grid_renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "seg/core/config.h"
#include "seg/gl/shader.h"

namespace seg {
namespace gl {
GridRenderer::GridRenderer() {
  // empty VAO for fullscreen quad (vertices generated in shader via gl_VertexID)
  glGenVertexArrays(1, &vao);
}

GridRenderer::~GridRenderer() { glDeleteVertexArrays(1, &vao); }

void GridRenderer::drawImpl() {
  if (getConfig().show_grid == false) return;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shader->bind();
  shader->setUniform("grid_color", grid_color.asEigenVector4f());
  shader->setUniform("grid_fade_distance", fade_distance);

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

}  // namespace gl
}  // namespace seg
