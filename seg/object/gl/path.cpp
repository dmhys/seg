#include "seg/object/gl/path.h"

#include <iostream>

#include <mutex>
#include <vector>

#include <imgui.h>

#include "seg/object/gl/basic_renderers.h"
#include "seg/object/gl/pose.h"

#include "seg/utilities/logger.h"

namespace seg {
namespace {  // anonymous namespace
const char* visual_types[] = {"NULL",      "Line",        "Axis",
                              "Axis/Line", "CameraFrame", "CameraFrames/Line"};

inline bool hasType(object::Path::VisualType type, object::Path::VisualType elem)
{
    return (static_cast<int>(type) & static_cast<int>(elem));
}
}  // anonymous namespace

namespace object {
Path::Path() : Path(std::vector<Eigen::Matrix4f>()) {}

Path::Path(std::vector<Eigen::Matrix4f> _poses) : poses(_poses)
{
    num_pose = _poses.size();

    line_renderer.reset(new LineRenderer());
    pose_renderer.reset(new Pose());
    if (poses.size() != 0) {
        std::vector<Eigen::Vector3f> vertices;
        vertices.reserve(poses.size());

        for (auto&& mat : poses) vertices.push_back(mat.block<3, 1>(0, 3));

        line_renderer->setData(std::move(vertices));
    }

    line_renderer->setLineWidth(line_width);
    line_renderer->setColor(line_color);

    pose_renderer->setLineWidth(axis_frame_line_width);
    pose_renderer->setColor(frame_color);

    inspector = ui::GeneralInspector::Builder()
                    .addField("Count", &num_pose)
                    .addDrawFunction([this] { drawInspector(); })
                    .build();
}

void Path::glFree()
{
    line_renderer->glFree();
    pose_renderer->glFree();
}

void Path::setShader(gl::Shader* _shader)
{
    line_renderer->setShader(_shader);
    pose_renderer->setShader(_shader);
}

void Path::reset()
{
    std::lock_guard<std::mutex> lock(pose_mtx);
    num_pose = 0;
    poses.clear();
    line_renderer->setData(std::vector<Eigen::Vector3f>());
}

void Path::reset(const std::vector<Eigen::Matrix4f> & _poses)
{
    auto copy = _poses;
    reset(std::move(copy));
}

void Path::reset(std::vector<Eigen::Matrix4f> && _poses)
{
    if (_poses.size() == 0) {
        reset();
        return;
    }

    std::lock_guard<std::mutex> lock(pose_mtx);
    num_pose = _poses.size();
    poses = std::move(_poses);
    std::vector<Eigen::Vector3f> vertices;
    vertices.reserve(poses.size());
    for (auto&& mat : poses) vertices.push_back(mat.block<3, 1>(0, 3));
    line_renderer->setData(std::move(vertices));
}

void Path::addPose(const Eigen::Matrix4f& pose)
{
    std::lock_guard<std::mutex> lock(pose_mtx);
    poses.push_back(pose);
    num_pose++;
    line_renderer->addData(pose.block<3, 1>(0, 3));
}

void Path::drawImpl()
{
    std::lock_guard<std::mutex> lock(pose_mtx);
    if (hasType(type, VisualType::LINE))
        line_renderer->draw();

    if (type == VisualType::LINE) return;

    if (hasType(type, VisualType::AXIS)) {
        pose_renderer->setVisualType(Pose::VisualType::AXIS);
    } else if (hasType(type, VisualType::CAMERA_FRAMES)) {
        pose_renderer->setVisualType(Pose::VisualType::CAMERA_FRAME);
    }

    for (auto&& pose : poses) {
        pose_renderer->setData(pose);
        pose_renderer->draw();
    }
}

void Path::drawInspector()
{
    static int color_edit_flag = 0;
    color_edit_flag |= ImGuiColorEditFlags_NoAlpha;
    color_edit_flag |= ImGuiColorEditFlags_NoSidePreview;
    color_edit_flag |= ImGuiColorEditFlags_PickerHueBar;
    color_edit_flag |= ImGuiColorEditFlags_DisplayRGB;

    if(ImGui::BeginCombo("Visualize", enumToCharP(type)))
    {
        if (ImGui::Selectable(enumToCharP(VisualType::LINE), type == VisualType::LINE))
            type = VisualType::LINE;

        if (ImGui::Selectable(enumToCharP(VisualType::AXIS), type == VisualType::AXIS))
            type = VisualType::AXIS;

        if (ImGui::Selectable(enumToCharP(VisualType::CAMERA_FRAMES), type == VisualType::CAMERA_FRAMES))
            type = VisualType::CAMERA_FRAMES;

        if (ImGui::Selectable(enumToCharP(VisualType::AXIS_WITH_LINE), type == VisualType::AXIS_WITH_LINE))
            type = VisualType::AXIS_WITH_LINE;

        if (ImGui::Selectable(enumToCharP(VisualType::CAMERA_FRAMES_WITH_LINE), type == VisualType::CAMERA_FRAMES_WITH_LINE))
            type = VisualType::CAMERA_FRAMES_WITH_LINE;

        ImGui::EndCombo();
    }

    if(hasType(type, VisualType::LINE))
    {
        if(ImGui::TreeNode("Line options"))
        {
            ImGui::SliderFloat("Thickness", &line_width, 1.0, 5.0, "%.1f");
            ImGui::ColorPicker3("##", (float*)&line_color, color_edit_flag);

            line_renderer->setLineWidth(line_width);
            line_renderer->setColor(line_color);

            ImGui::TreePop();
        }
    }

    if(hasType(type, VisualType::AXIS))
    {
        if(ImGui::TreeNode("Axis Options"))
        {
            ImGui::SliderFloat("Scale", &pose_renderer->scale, 0.1, 5.0, "%.1f");
            ImGui::SliderFloat("Thickness", &pose_renderer->line_width, 1.0, 5.0, "%.1f");

            ImGui::TreePop();
        }
    }

    if(hasType(type, VisualType::CAMERA_FRAMES))
    {
        if(ImGui::TreeNode("Frame Options"))
        {
            ImGui::SliderFloat("Scale", &pose_renderer->scale, 0.1, 5.0, "%.1f");
            ImGui::SliderFloat("Thickness", &pose_renderer->line_width, 1.0, 5.0, "%.1f");

            ImGui::ColorPicker3("##", (float*)&frame_color, color_edit_flag);

            pose_renderer->setColor(frame_color);
            
            ImGui::TreePop();
        }
    }
}
}  // namespace object

const char* enumToCharP(object::Path::VisualType type)
{
    return visual_types[static_cast<int>(type)];
}

}  // namespace seg