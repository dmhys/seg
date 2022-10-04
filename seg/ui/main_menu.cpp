#include "seg/ui/main_menu.h"

#include <imgui.h>
#include <portable-file-dialogs.h>

#include "seg/core/config.h"
#include "seg/object/ui_object.h"
#include "seg/utilities/logger.h"

bool show_demowindow = false;

namespace seg {
namespace ui {

MainMenu::~MainMenu()
{
    if (about_window) about_window->kill();
}

void MainMenu::drawImpl()
{
    if (show_demowindow) ImGui::ShowDemoWindow();

    ImGui::BeginMainMenuBar();

    fileMenu();

    visualizeMenu();

    windowMenu();

    about();

    ImGui::EndMainMenuBar();
}

void MainMenu::fileMenu()
{
    if (ImGui::BeginMenu("File")) {
        ImGui::Text("Settings");
        ImGui::Indent();
        ImGui::Text("Save(TODO:)");
        ImGui::Text("Load(TODO:)");

        ImGui::Unindent();

        ImGui::Separator();  // -----------------

        ImGui::Text("Load File(TODO:)");

        ImGui::EndMenu();
    }
}

void MainMenu::visualizeMenu()
{
    if (ImGui::BeginMenu("Visualize")) {
        static bool is_light = getConfig().theme == Theme::LIGHT;
        static bool is_dark = !is_light;

        ImGui::Text("Theme");
        ImGui::Indent();
        if (ImGui::MenuItem("LIGHT", nullptr, &is_light)) {
            if (is_light) {
                is_dark = false;
                getConfig().theme = Theme::LIGHT;
                ImGui::StyleColorsLight();
            } else
                is_light = true;
        }

        if (ImGui::MenuItem("DARK", nullptr, &is_dark)) {
            if (is_dark) {
                is_light = false;
                getConfig().theme = Theme::DARK;
                ImGui::StyleColorsDark();
            } else
                is_dark = true;
        }
        ImGui::Unindent();

        ImGui::Separator();  // -----------------

        ImGui::Text("Show");
        ImGui::Indent();
        ImGui::MenuItem("FPS", nullptr, &getConfig().show_fps);
        ImGui::MenuItem("Grid", nullptr, &getConfig().show_grid);
        ImGui::MenuItem("Origin Axis", nullptr, &getConfig().show_origin);
        ImGui::Unindent();

        ImGui::EndMenu();
    }
}

void MainMenu::windowMenu()
{
    if (ImGui::BeginMenu("Window")) {
        ImGui::MenuItem("show_demo", nullptr, &show_demowindow);

        ImGui::MenuItem("Objects", nullptr, show_object_list);

        ImGui::MenuItem("Insepector", nullptr, show_object_inspector);

        ImGui::EndMenu();
    }
}

void MainMenu::about()
{
    if (ImGui::MenuItem("About")) {
        if (about_window) about_window->kill();

        about_window.reset(new pfd::message("About",
                                            "SEG - SLAM EASY GUI\n (BSD 3-Clause "
                                            "License) \nhttps://github.com/dmhys/seg.git \ndmhys@naver.com",
                                            pfd::choice::ok, pfd::icon::info));
    }
}

}  // namespace ui
}  // namespace seg