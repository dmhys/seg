#include "seg/gl/scene.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Eigen/Dense>

#include "seg/core/config.h"
#include "seg/gl/grid_renderer.h"
#include "seg/utilities/logger.h"

seg::gl::Camera* cam_ptr;
seg::gl::Scene* scene_ptr;

void windowResizeCb(GLFWwindow* window, int width, int height)
{
    cam_ptr->onScreenResize(width, height);
    scene_ptr->onScreenResize(width, height);
}

namespace seg {
namespace gl {
void Scene::init(GLFWwindow* window)
{
    // shader
    shader.init();

    // camera
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    camera.init(width, height);

    // gl
    glViewport(0, 0, width, height);

    // Callbacks
    cam_ptr = &camera;
    scene_ptr = this;
    glfwSetWindowSizeCallback(window, windowResizeCb);

    // base object - grid
    auto grid_renderer = new GridRenderer();
    grid_renderer->setShader(&shader);
    base_objects.emplace_back(std::unique_ptr<GridRenderer>(grid_renderer));
}

void Scene::draw()
{
    clear();

    shader.bind();

    updateVpMatrix();

    for (auto& object_ptr : base_objects) object_ptr->draw();

    shader.unbind();
}

void Scene::clearObjects() { base_objects.clear(); }

void Scene::onScreenResize(int width, int height) { glViewport(0, 0, width, height); }

void Scene::clear()
{
    const auto& clear_color = enumToStruct(getConfig().theme);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::updateVpMatrix()
{
    Eigen::Matrix4f vp_matrix = camera.getProjectionMatrix() * camera.getViewMatrix();
    shader.setUniform("vp_matrix", vp_matrix);
}

}  // namespace gl
}  // namespace seg