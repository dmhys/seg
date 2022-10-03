#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <Eigen/Dense>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "seg/gl/general_renderer.h"
#include "seg/gl/shader.h"
#include "seg/object/gl/basic_renderers.h"
#include "seg/ui/general_inspector.h"

namespace seg {
namespace object {
StaticLineRenderer::StaticLineRenderer(
    const std::tuple<std::vector<Eigen::Vector3f>, std::vector<Eigen::Vector3f>>
        &vertices_colors)
    : StaticLineRenderer(std::get<0>(vertices_colors), std::get<1>(vertices_colors))
{
}

StaticLineRenderer::StaticLineRenderer(const std::vector<Eigen::Vector3f> &vertices,
                                       const std::vector<Eigen::Vector3f> &colors)
{
    if (vertices.empty())
        throw std::invalid_argument("StaticLineRenderer - Given Vertices empty.");

    pimpl.reset(new gl::GeneralRenderer(gl::GeneralRenderer::BufferType::STATIC,
                                        gl::GeneralRenderer::RenderTarget::LINE));

    std::vector<Eigen::Vector3f> tmp_vertices = vertices;  // clone
    std::vector<Eigen::Vector3f> tmp_colors = colors;      // clone
    pimpl->setData(std::move(tmp_vertices), std::move(tmp_colors));

    inspector = ui::GeneralInspector::Builder()
                    .addField("Vertex", &pimpl->vertexCount())
                    .addDrawFunction([this] {
                        ImGui::PushItemWidth(ui::GeneralInspector::DEFAULT_ITEM_WIDTH);
                        ImGui::SliderFloat("line width", &line_width, 1.0, 5.0, "%.1f");
                    })
                    .build();
}

void StaticLineRenderer::drawImpl()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shader->setUniform("model_matrix", model_matrix);
    shader->setUniform("color_mode", 0);
    shader->setUniform("uniform_color", color.asEigenVector4f());

    glLineWidth(line_width);
    pimpl->draw(shader);
}

}  // namespace object
}  // namespace seg