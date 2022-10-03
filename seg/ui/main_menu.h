#ifndef SEG_UI_MAIN_MENU_H
#define SEG_UI_MAIN_MENU_H

#include <portable-file-dialogs.h>

#include <memory>
#include <string>

#include "seg/object/ui_object.h"

namespace seg {
namespace ui {
class MainMenu : public object::UIObject
{
   public:
    MainMenu(bool* _show_object_list, bool* _show_object_inspector)
        : show_object_list(_show_object_list), show_object_inspector(_show_object_inspector)
    {
    }
    ~MainMenu();
    const std::string getType() const override { return "Main Menu"; }

   private:
    void drawImpl() override;

    void fileMenu();
    void visualizeMenu();
    void windowMenu();
    void about();

    bool* show_object_list;
    bool* show_object_inspector;

    std::unique_ptr<pfd::message> about_window;

};  // class MainMenu
}  // namespace ui
}  // namespace seg

#endif