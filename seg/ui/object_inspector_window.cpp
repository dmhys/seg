#include "seg/ui/object_inspector_window.h"

#include <imgui.h>

#include <string>

#include "seg/core/config.h"
#include "seg/ui/general_inspector.h"

namespace seg {
namespace ui {
ObjectInspectorWindow::ObjectInspectorWindow() { window_flag |= ImGuiWindowFlags_NoCollapse; }

void ObjectInspectorWindow::drawImpl()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200, 200));
    ImGui::Begin("Inspector", nullptr, window_flag);

    if (getConfig().selected_object != nullptr) {
        ImGui::TextUnformatted(getConfig().selected_object_name.c_str());

        ImGui::Separator();  // -----------------

        ImGui::TextUnformatted(
            ("Type : " + getConfig().selected_object->getType()).c_str());

        auto inspector = getConfig().selected_object->inspector.get();
        if (inspector != nullptr) inspector->draw();
    }

    ImGui::End();
    ImGui::PopStyleVar(1);
}

}  // namespace ui
}  // namespace seg