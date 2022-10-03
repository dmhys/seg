#include "seg/object/gl/pose.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <Eigen/Dense>

#include "seg/gl/general_renderer.h"
#include "seg/gl/shader.h"
#include "seg/object/primitives.h"
#include "seg/ui/general_inspector.h"

#include "seg/utilities/logger.h"

namespace seg {

namespace {  // anonymous namespace
const char* visual_types[] = {"AXIS", "Camera Frame"};
}

namespace object {
Pose::Pose() : Pose(Eigen::Matrix4f::Identity()) {}

Pose::Pose(const Eigen::Matrix4f _pose) : pose(_pose)
{
    pimpl.reset(new gl::GeneralRenderer(gl::GeneralRenderer::BufferType::STATIC,
                                        gl::GeneralRenderer::RenderTarget::LINE));

    camera_frame = primitives::CameraFrame();
    axis = primitives::Axis();

    auto copy = axis;
    pimpl->setData(std::move(std::get<0>(copy)), std::move(std::get<1>(copy)));

    inspector = ui::GeneralInspector::Builder()
                    .addDrawFunction([this] { drawInspector(); })
                    .build();
}

void Pose::setCameraParameters(float width, float height, float focal_length)
{
    camera_frame = primitives::CameraFrame(width, height, focal_length);
}

void Pose::setCameraParameters(float width, float height, float fx, float fy, float cx,
                               float cy)
{
    camera_frame = primitives::CameraFrame(width, height, fx, fy, cx, cy);
}

void Pose::setData(Eigen::Matrix4f&& _pose)
{
    std::lock_guard<std::mutex> lock(pose_mtx);
    pose = _pose;
}

void Pose::setData(const Eigen::Matrix4f& _pose)
{
    Eigen::Matrix4f tmp_pose = _pose;
    setData(std::move(tmp_pose));
}

void Pose::drawInspector()
{
    static int color_edit_flag = 0;
    color_edit_flag |= ImGuiColorEditFlags_NoAlpha;
    color_edit_flag |= ImGuiColorEditFlags_NoSidePreview;
    color_edit_flag |= ImGuiColorEditFlags_PickerHueBar;
    color_edit_flag |= ImGuiColorEditFlags_DisplayRGB;

    ImGui::SliderFloat("Scale", &scale, 0.1, 5.0, "%.1f");
    ImGui::SliderFloat("Thickness", &line_width, 1.0, 5.0, "%.1f");

    if (ImGui::BeginCombo("Visualize", enumToCharP(type))) {
        if (ImGui::Selectable(enumToCharP(VisualType::AXIS), type == VisualType::AXIS))
            type = VisualType::AXIS;

        if (ImGui::Selectable(enumToCharP(VisualType::CAMERA_FRAME),
                              type == VisualType::CAMERA_FRAME))
            type = VisualType::CAMERA_FRAME;

        ImGui::EndCombo();
    }

    if (type == VisualType::CAMERA_FRAME) {
        if (ImGui::TreeNode("Color Picker")) {
            ImGui::ColorPicker3("##", (float*)&color, color_edit_flag);
            ImGui::TreePop();
        }
    }
}

void Pose::drawImpl()
{
    pose_mtx.lock();
    model_matrix.block<3, 3>(0, 0) = pose.block<3, 3>(0, 0) * scale;
    model_matrix.block<3, 1>(0, 3) = pose.block<3, 1>(0, 3);
    pose_mtx.unlock();

    if (prev != type) {
        if (type == VisualType::AXIS) {
            auto copy = axis;
            pimpl->setData(std::move(std::get<0>(copy)), std::move(std::get<1>(copy)));
        } else {
            auto tmp = camera_frame;
            pimpl->setData(std::move(tmp));
        }

        prev = type;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shader->setUniform("model_matrix", model_matrix);

    if (type == VisualType::AXIS)
        shader->setColorMode(gl::ColorMode::RGB);
    else {
        shader->setColorMode(gl::ColorMode::UNIFORM);
        shader->setUniform("uniform_color", color.asEigenVector4f());
    }

    glLineWidth(line_width);
    pimpl->draw(shader);
}

}  // namespace object

const char* enumToCharP(object::Pose::VisualType type)
{
    return visual_types[static_cast<int>(type)];
}
}  // namespace seg