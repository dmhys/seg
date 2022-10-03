#include "seg/ui/controller.h"

#include <imgui.h>

#include <memory>

#include "seg/gl/scene.h"
#include "seg/object/ui_object.h"
#include "seg/ui/fps_overlay.h"
#include "seg/ui/main_menu.h"
#include "seg/ui/object_inspector_window.h"
#include "seg/ui/object_list_window.h"
#include "seg/utilities/logger.h"

namespace seg {
namespace ui {
void Controller::init(gl::Scene *_scene, object::ObjectManager *_object_manager)
{
    scene = _scene;
    object_manager = _object_manager;

    ObjectListWindow *obj_list = new ObjectListWindow(object_manager);
    ObjectInspectorWindow *obj_inspector = new ObjectInspectorWindow();

    base_objects.emplace_back(
        std::make_unique<MainMenu>(&obj_list->is_visible, &obj_inspector->is_visible));
    base_objects.emplace_back(std::make_unique<FpsOverlay>());
    base_objects.push_back(std::move(std::unique_ptr<ObjectListWindow>(obj_list)));
    base_objects.push_back(std::move(std::unique_ptr<ObjectInspectorWindow>(obj_inspector)));
}

void Controller::drawUI()
{
    ImGuiID dockspace = ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport(),
                                                     ImGuiDockNodeFlags_PassthruCentralNode);

    handleMouseEvents();

    for (auto &object_ptr : base_objects) object_ptr->draw();
}

void Controller::handleMouseEvents()
{
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)  // mouse hovering on GUI
        return;

    const auto mouse_pos = ImGui::GetMousePos();
    for (int i = 0; i < 3; i++)  // 0 left // 1 right // 2 wheel
    {
        if (ImGui::IsMouseClicked(i)) scene->camera.onMouseClick(i, mouse_pos.x, mouse_pos.y);
        if (ImGui::IsMouseDragging(i)) scene->camera.onMouseDrag(mouse_pos.x, mouse_pos.y);
    }
    if (io.MouseWheel != 0) scene->camera.onMouseWheel(io.MouseWheel);
}

}  // namespace ui
}  // namespace seg