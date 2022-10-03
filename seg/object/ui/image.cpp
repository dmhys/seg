#include "seg/object/ui/image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <mutex>

#include "seg/utilities/logger.h"
#include "seg/ui/general_inspector.h"

namespace {  // anonymous namespace
void getImageFitPosAndSize(int image_width, int image_height, ImVec2& image_cursor_pos,
                         ImVec2& image_disp_size)
{
    ImVec2 content_area_start = ImGui::GetWindowContentRegionMin();
    ImVec2 content_area_end = ImGui::GetWindowContentRegionMax();
    ImVec2 content_area_size(content_area_end.x - content_area_start.x,
                             content_area_end.y - content_area_start.y);

    const float aspect_window = content_area_size.x / content_area_size.y;
    const float aspect_image =
        static_cast<float>(image_width) / static_cast<float>(image_height);

    if (aspect_window > aspect_image)  // letterbox - side
    {
        const int disp_size_x = static_cast<int>(content_area_size.y * aspect_image);
        const int margin_side = static_cast<int>(content_area_size.x) - disp_size_x;
        image_cursor_pos =
            ImVec2(content_area_start.x + margin_side / 2, content_area_start.y);
        image_disp_size = ImVec2(disp_size_x, content_area_size.y);
    } else  // letterbox - updown
    {
        const int disp_size_y = static_cast<int>(content_area_size.x / aspect_image);
        const int margin_updown = static_cast<int>(content_area_size.y) - disp_size_y;
        image_cursor_pos =
            ImVec2(content_area_start.x, content_area_start.y + margin_updown / 2);
        image_disp_size = ImVec2(content_area_size.x, disp_size_y);
    }
}
}  // namespace

namespace seg {
namespace object {

Image::Image(std::string _name) : name(_name)
{
    window_flag |= ImGuiWindowFlags_NoScrollbar;

    auto tmp = cv::Mat(cv::Size2i(200, 200), CV_8UC3, cv::Scalar(0, 0, 0));
    setData(std::move(tmp));
    
    inspector = ui::GeneralInspector::Builder()
                    .addField("Width ", &width)
                    .addField("Height", &height)
                    .build();
}

void Image::setData(void* image_buffer, ImageSize size, ImageType type)
{
    LOG_WARN("NOT YET IMPLEMENTED!");
}

void Image::drawImpl()
{
    if (texture_id == 0)  // this stage texture id cannot be 0
        glGenTextures(1, &texture_id);

    if (updated.load() == true) updateTexture();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200, 200));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    // window ==========================================
    ImGui::Begin((name + "##" + std::to_string(texture_id)).c_str(), &is_visible,
                 window_flag);

    ImVec2 image_cursor_pos;
    ImVec2 image_disp_size;
    getImageFitPosAndSize(width, height, image_cursor_pos, image_disp_size);

    ImGui::SetCursorPos(image_cursor_pos);
    ImGui::Image((void*)(intptr_t)texture_id, image_disp_size);

    ImGui::End();
    // =================================================

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

#ifdef OPENCV
void Image::setData(cv::Mat&& image)
{
    auto lock = std::unique_lock<std::mutex>(mtx);
    image_buffer = std::move(image); // cv::Mat supports move scemantic
    updated.store(true);
}

void Image::setData(const cv::Mat& image)
{
    setData(image.clone());
}

void Image::updateTexture()
{
    auto lock = std::unique_lock<std::mutex>(mtx);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    if(image_buffer.channels()==3)
        cv::cvtColor(image_buffer, image_buffer, cv::COLOR_BGR2RGB);
    else if(image_buffer.channels()==1)
        cv::cvtColor(image_buffer,image_buffer, cv::COLOR_GRAY2RGB);

    width = image_buffer.cols;
    height = image_buffer.rows;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_buffer.cols, image_buffer.rows, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image_buffer.ptr());

    updated.store(false);
}

#endif
}
}  // namespace seg