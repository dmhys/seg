#include "seg/ui/general_inspector.h"

#include <functional>
#include <stdexcept>
#include <string>

#include <imgui.h>

#include "seg/utilities/logger.h"

namespace seg {
namespace ui {

void GeneralInspector::draw() {
  drawFields();

  ImGui::Separator();  // -----------------

  drawFunctions();
}

void GeneralInspector::drawFields() {
  if (ImGui::TreeNode("Fields")) {
    for (auto& field : fields) {
      ImGui::TextUnformatted((field.name + " : " + field.display()).c_str());
    }

    ImGui::TreePop();
  }
}

void GeneralInspector::drawFunctions() {
  ImGui::PushItemWidth(DEFAULT_ITEM_WIDTH);

  try {
    for (auto&& function : functions) {
      function();
    }
  } catch (const std::exception& e) {
    LOG_ERROR("General Inpector - Error While executing given draw Function!");
    LOG_ERROR(e.what());
  }
}

}  // namespace ui
}  // namespace seg