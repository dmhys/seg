#include "seg/ui/fps_overlay.h"

#include <imgui.h>

#include "seg/core/config.h"
#include "seg/utilities/logger.h"

namespace seg {
namespace ui {
FpsOverlay::FpsOverlay()
{
    window_flag |= ImGuiWindowFlags_NoDecoration;
    window_flag |= ImGuiWindowFlags_AlwaysAutoResize;
    window_flag |= ImGuiWindowFlags_NoSavedSettings;
    window_flag |= ImGuiWindowFlags_NoFocusOnAppearing;
    window_flag |= ImGuiWindowFlags_NoNav;
    window_flag |= ImGuiWindowFlags_NoMove;
}

void FpsOverlay::drawImpl()
{
    if (getConfig().show_fps == false) return;

    ImGuiIO& io = ImGui::GetIO();
    const float padding = 10.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos;
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;

    // upper left
    // window_pos.x = (work_pos.x + padding);
    // window_pos.y = (work_pos.y + padding);
    // window_pos_pivot.x = 0.0f;
    // window_pos_pivot.y = 0.0f;

    // upper right
    window_pos.x = (work_pos.x + work_size.x - padding);
    window_pos.y = (work_pos.y + padding);
    window_pos_pivot.x = 1.0f;
    window_pos_pivot.y = 0.0f;

    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);

    ImGui::Begin(" ", nullptr, window_flag);
    ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 0.9f), "FPS : %.1f", io.Framerate);

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}

}  // namespace ui
}  // namespace seg