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

StaticPointcloudRenderer::StaticPointcloudRenderer(
    const std::vector<Eigen::Vector3f> &vertices, const std::vector<float> &scalars)
    : StaticPointcloudRenderer(vertices, std::vector<Eigen::Vector3f>(), scalars)
{
}

StaticPointcloudRenderer::StaticPointcloudRenderer(
    const std::tuple<std::vector<Eigen::Vector3f>, std::vector<Eigen::Vector3f>>
        &vertices_colors)
    : StaticPointcloudRenderer(std::get<0>(vertices_colors), std::get<1>(vertices_colors))
{
}

StaticPointcloudRenderer::StaticPointcloudRenderer(
    const std::vector<Eigen::Vector3f> &vertices,
    const std::vector<Eigen::Vector3f> &colors, const std::vector<float> &scalars)
{
    if (vertices.empty())
        throw std::invalid_argument("StaticPointcloudRenderer - Given Vertices empty.");

    pimpl.reset(new gl::GeneralRenderer(gl::GeneralRenderer::BufferType::STATIC,
                                        gl::GeneralRenderer::RenderTarget::POINT));

    // clone
    std::vector<Eigen::Vector3f> tmp_vertices = vertices;
    std::vector<Eigen::Vector3f> tmp_colors = colors;
    std::vector<float> tmp_scalars = scalars;
    pimpl->setData(std::move(tmp_vertices), std::move(tmp_colors),
                      std::move(tmp_scalars));

    inspector = ui::GeneralInspector::Builder()
                    .addField("Vertices", &pimpl->vertexCount())
                    .addField("Has Scalar", &pimpl->hasScalar())
                    .addField("Has Color", &pimpl->hasColor())
                    .addDrawFunction([this] { drawInspector(); })
                    .build();
}

void StaticPointcloudRenderer::drawImpl()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shader->setModelMatrix(model_matrix);
    shader->setColorMode(color_mode);
    shader->setUniform("uniform_color", color.asEigenVector4f());

    switch(color_mode)
    {
        case gl::ColorMode::UNIFORM:
            shader->setUniform("visualize_z_min", visualize_z_min);
            shader->setUniform("visualize_z_max", visualize_z_max);
        case gl::ColorMode::SCALAR:
            shader->setUniform("visualize_hue_from", visualize_hue_from);
            shader->setUniform("visualize_hue_to", visualize_hue_to);
            shader->setUniform("visualize_saturation", visualize_saturation);
            shader->setUniform("visualize_value", visualize_value);
    }
    
    glPointSize(point_size);
    pimpl->draw(shader);
}

void StaticPointcloudRenderer::drawInspector()
{
    static int color_edit_flag = 0;
    color_edit_flag |= ImGuiColorEditFlags_NoAlpha;
    color_edit_flag |= ImGuiColorEditFlags_NoSidePreview;
    color_edit_flag |= ImGuiColorEditFlags_PickerHueBar;
    color_edit_flag |= ImGuiColorEditFlags_DisplayRGB;

    ImGui::SliderFloat("Point Size", &point_size, 1.0, 5.0, "%.1f");

    // Color mode selection
    if (ImGui::BeginCombo("Coloring", enumToCharP(color_mode))) {
        if (ImGui::Selectable(enumToCharP(gl::ColorMode::UNIFORM),
                              color_mode == gl::ColorMode::UNIFORM))
            color_mode = gl::ColorMode::UNIFORM;

        if (pimpl->hasColor()) {
            if (ImGui::Selectable(enumToCharP(gl::ColorMode::RGB),
                                  color_mode == gl::ColorMode::RGB))
                color_mode = gl::ColorMode::RGB;
        }

        if (pimpl->hasScalar()) {
            if (ImGui::Selectable(enumToCharP(gl::ColorMode::SCALAR),
                                  color_mode == gl::ColorMode::SCALAR))
                color_mode = gl::ColorMode::SCALAR;
        }

        if (ImGui::Selectable(enumToCharP(gl::ColorMode::ZAXIS),
                              color_mode == gl::ColorMode::ZAXIS))
            color_mode = gl::ColorMode::ZAXIS;

        ImGui::EndCombo();
    }

    if (color_mode == gl::ColorMode::UNIFORM) {
        if (ImGui::TreeNode("Color Picker")) {
            ImGui::ColorPicker3("##", (float *)&color, color_edit_flag);
            ImGui::TreePop();
        }
    }
    else if (color_mode == gl::ColorMode::ZAXIS) {
        if (ImGui::TreeNode("Color Options"))
        {
            ImGui::SliderFloat("Hue From", &visualize_hue_from, 0.0f, 1.0f, "Hue From   = %.2f");
            ImGui::SliderFloat("Hue To", &visualize_hue_to, 0.0f, 1.0f, "Hue To     = %.2f");
            ImGui::SliderFloat("Saturation", &visualize_saturation, 0.0f, 1.0f, "Saturation = %.2f");
            ImGui::SliderFloat("Value", &visualize_value, 0.0f, 1.0f, "Value      = %.2f");

            ImGui::SliderFloat("Z Min", &visualize_z_min, -30.0f, 0.0f, "min z = %.2f");
            ImGui::SliderFloat("Z Max", &visualize_z_max, 0.0f, 200.0f, "max z = %.2f");

            ImGui::TreePop();
        }
    }
}

}  // namespace object
}  // namespace seg