#include "seg/ui/object_list_window.h"

#include <iostream>

#include <imgui.h>

#include "seg/core/config.h"
#include "seg/core/object_manager.h"

namespace seg {
namespace ui {
ObjectListWindow::ObjectListWindow(object::ObjectManager* _object_manager)
    : object_manager(_object_manager) {
  window_flag |= ImGuiWindowFlags_NoCollapse;
}

void ObjectListWindow::drawImpl() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200, 200));

  ImGui::Begin("Objects", nullptr, window_flag);

  int i = -1;  // to resolve id collision
  for (auto& object : object_manager->objects) {
    i++;
    const bool is_selected = (object.first == getConfig().selected_object_name);
    ImGui::Checkbox((std::string("##") + std::to_string(i)).c_str(),
                    &object.second.get()->is_visible);
    ImGui::SameLine();
    if (ImGui::Selectable(object.first.c_str(), is_selected)) {
      getConfig().selected_object_name = object.first;
    }
  }  // TODO: implement object deselection when clicking empty area

  ImGui::End();

  ImGui::PopStyleVar(1);
}

}  // namespace ui
}  // namespace seg