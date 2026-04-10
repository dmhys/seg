#include "seg/gl/scene.h"

#include <Eigen/Dense>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "seg/core/config.h"
#include "seg/gl/grid_renderer.h"
#include "seg/internal/logger.h"

namespace seg {
namespace gl {
void Scene::init(GLFWwindow* window) {
  // shaders
  general_shader.init(ShaderType::GENERAL);
  grid_shader.init(ShaderType::GRID);

  // camera
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  camera.init(width, height);

  // gl
  int fb_width, fb_height;
  glfwGetFramebufferSize(window, &fb_width, &fb_height);
  glViewport(0, 0, fb_width, fb_height);

  // Callbacks — use GLFW user pointer instead of global statics
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(
      window, [](GLFWwindow* w, int fb_width, int fb_height) {
        auto* scene = static_cast<Scene*>(glfwGetWindowUserPointer(w));
        int win_width, win_height;
        glfwGetWindowSize(w, &win_width, &win_height);
        scene->camera.onScreenResize(win_width, win_height);
        scene->onScreenResize(fb_width, fb_height);
      });

  // base object - grid
  auto grid_renderer = new GridRenderer();
  grid_renderer->setShader(&grid_shader);
  base_objects.emplace_back(std::unique_ptr<GridRenderer>(grid_renderer));
}

void Scene::draw() {
  clear();
  updateVpMatrix();

  for (auto& object_ptr : base_objects) object_ptr->draw();
}

void Scene::clearObjects() { base_objects.clear(); }

void Scene::onScreenResize(int width, int height) {
  glViewport(0, 0, width, height);
}

void Scene::clear() {
  const auto& clear_color = enumToStruct(getConfig().theme);
  glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::updateVpMatrix() {
  Eigen::Matrix4f vp_matrix =
      camera.getProjectionMatrix() * camera.getViewMatrix();

  general_shader.bind();
  general_shader.setUniform("vp_matrix", vp_matrix);

  Eigen::Matrix4f inv_vp_matrix = vp_matrix.inverse();
  grid_shader.bind();
  grid_shader.setUniform("vp_matrix", vp_matrix);
  grid_shader.setUniform("inv_vp_matrix", inv_vp_matrix);
}

}  // namespace gl
}  // namespace seg