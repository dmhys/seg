#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Eigen/Dense>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "seg/gl/general_renderer.h"
#include "seg/gl/shader.h"
#include "seg/object/gl/basic_renderers.h"
#include "seg/ui/general_inspector.h"
#include "seg/utilities/logger.h"

namespace seg {
namespace object {
StaticMeshRenderer::StaticMeshRenderer(
    const std::tuple<std::vector<Eigen::Vector3f>, std::vector<Triangle>>
        vertices_triangles)
{
    if (std::get<0>(vertices_triangles).empty())
        throw std::invalid_argument("StaticMeshRenderer - Given Vertices empty.");

    pimpl.reset(new gl::GeneralRenderer(gl::GeneralRenderer::BufferType::STATIC,
                                        gl::GeneralRenderer::RenderTarget::TRIANGLES));

    std::vector<Eigen::Vector3f> tmp_vertices = std::get<0>(vertices_triangles);  // clone
    std::vector<Triangle> tmp_indicies = std::get<1>(vertices_triangles);         // clone
    pimpl->setData(std::move(tmp_vertices), std::move(tmp_indicies));
}

void StaticMeshRenderer::drawImpl()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shader->setUniform("model_matrix", model_matrix);
    shader->setUniform("color_mode", 0);
    shader->setUniform("uniform_color", color.asEigenVector4f());

    pimpl->draw(shader);
}

}  // namespace object
}  // namespace seg