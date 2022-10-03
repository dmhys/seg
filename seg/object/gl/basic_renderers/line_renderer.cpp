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
#include "seg/utilities/logger.h"

namespace seg {
namespace object {

LineRenderer::LineRenderer()
{
    pimpl.reset(new gl::GeneralRenderer(gl::GeneralRenderer::BufferType::DYNAMIC,
                                        gl::GeneralRenderer::RenderTarget::LINE_STRIP));

    static int color_edit_flag = 0;
    color_edit_flag |= ImGuiColorEditFlags_NoAlpha;
    color_edit_flag |= ImGuiColorEditFlags_NoSidePreview;
    color_edit_flag |= ImGuiColorEditFlags_PickerHueBar;
    color_edit_flag |= ImGuiColorEditFlags_DisplayRGB;

    inspector = ui::GeneralInspector::Builder()
                    .addField("Vertices", &pimpl->vertexCount())
                    .addDrawFunction([this] {
                        ImGui::SliderFloat("Thickness", &line_width, 1.0, 5.0, "%.1f");
                        if (ImGui::TreeNode("Color Picker")) {
                            ImGui::ColorPicker3("##", (float*)&color, color_edit_flag);
                            ImGui::TreePop();
                        }
                    })
                    .build();
}

void LineRenderer::addData(const Eigen::Vector3f & vertex)
{
    vertices.push_back(vertex);

    auto copy = vertices;
    pimpl->setData(std::move(copy));
}

void LineRenderer::setData(std::vector<Eigen::Vector3f>&& _vertices)
{
    vertices = std::move(_vertices);
    auto copy = vertices;

    pimpl->setData(std::move(copy));
}

void LineRenderer::setData(const std::vector<Eigen::Vector3f>& _vertices)
{
    vertices = _vertices;
    auto copy = _vertices;

    pimpl->setData(std::move(copy));
}

void LineRenderer::drawImpl()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shader->setModelMatrix(model_matrix);
    shader->setColorMode(gl::ColorMode::UNIFORM);
    shader->setUniform("uniform_color",color.asEigenVector4f());

    glLineWidth(line_width);
    pimpl->draw(shader);
}

}
}  // namespace seg