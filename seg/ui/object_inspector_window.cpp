#include "seg/ui/object_inspector_window.h"

#include <string>

#include <imgui.h>

#include "seg/core/config.h"
#include "seg/core/object_manager.h"
#include "seg/object/object_base.h"
#include "seg/ui/general_inspector.h"

namespace seg {
namespace ui {
ObjectInspectorWindow::ObjectInspectorWindow(object::ObjectManager* om)
    : object_manager(om) {
  window_flag |= ImGuiWindowFlags_NoCollapse;
}

void ObjectInspectorWindow::drawImpl() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200, 200));
  ImGui::Begin("Inspector", nullptr, window_flag);

  auto& name = getConfig().selected_object_name;
  if (!name.empty()) {
    auto obj = object_manager->getObject(name).lock();
    if (obj) {
      ImGui::TextUnformatted(name.c_str());
      ImGui::Separator();
      ImGui::TextUnformatted(("Type : " + obj->getType()).c_str());
      if (obj->inspector) obj->inspector->draw();
    }
  }

  ImGui::End();
  ImGui::PopStyleVar(1);
}

}  // namespace ui
}  // namespace seg