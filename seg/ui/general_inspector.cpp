#include "seg/ui/general_inspector.h"

#include <imgui.h>

#include <functional>
#include <stdexcept>
#include <string>
#include <typeinfo>

#include "seg/utilities/logger.h"

namespace seg {
namespace ui {

void GeneralInspector::draw()
{
    drawFields();

    ImGui::Separator();  // -----------------
    
    drawFunctions();
}

void GeneralInspector::drawFields()
{
    if (ImGui::TreeNode("Fields")) {
        for (auto&& field : fields) {
            if (field.type() == &typeid(int*))
                ImGui::Text((field.name() + " : %d").c_str(),
                            *(static_cast<int*>(field.pointer())));
            else if (field.type() == &typeid(unsigned int*))
                ImGui::Text((field.name() + " : %d").c_str(),
                            *(static_cast<unsigned int*>(field.pointer())));
            else if (field.type() == &typeid(long*))
                ImGui::Text((field.name() + " : %d").c_str(),
                            *(static_cast<long*>(field.pointer())));
            else if (field.type() == &typeid(unsigned long*))
                ImGui::Text((field.name() + " : %d").c_str(),
                            *(static_cast<unsigned long*>(field.pointer())));
            else if (field.type() == &typeid(float*))
                ImGui::Text((field.name() + " : %.1f").c_str(),
                            *(static_cast<float*>(field.pointer())));
            else if (field.type() == &typeid(double*))
                ImGui::Text((field.name() + " : %.1f").c_str(),
                            *(static_cast<double*>(field.pointer())));
            else if (field.type() == &typeid(bool *))
                ImGui::TextUnformatted(
                    (field.name() + " : " +
                     (*static_cast<bool*>(field.pointer()) ? "True" : "False"))
                        .c_str());
            else if (field.type() == &typeid(std::string*))
                ImGui::TextUnformatted(
                    (field.name() + " : " + *(static_cast<std::string*>(field.pointer())))
                        .c_str());
            else {
                ImGui::TextUnformatted((field.name() + " : unsupported").c_str());
                LOG_WARN("Not supported type : {}", field.type()->name());
            }
        }

        ImGui::TreePop();
    }
}

void GeneralInspector::drawFunctions()
{
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