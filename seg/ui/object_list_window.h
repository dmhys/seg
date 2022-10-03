#ifndef SEG_UI_OBJECT_LIST_WINDOW_H
#define SEG_UI_OBJECT_LIST_WINDOW_H

#include <string>

#include "seg/object/ui_object.h"

namespace seg {
namespace object {
class ObjectManager;
};

namespace ui {
class ObjectListWindow : public object::UIObject
{
   public:
    ObjectListWindow(object::ObjectManager* object_manager);

    const std::string getType() const override { return "Object List Window"; }

   private:
    void drawImpl() override;

    object::ObjectManager* object_manager;

};  // class ObjectListWindow
}  // namespace ui
}  // namespace seg

#endif