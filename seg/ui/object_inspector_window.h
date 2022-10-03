#ifndef SEG_UI_OBJECT_INSPECTOR_WINDOW_H
#define SEG_UI_OBJECT_INSPECTOR_WINDOW_H

#include <string>

#include "seg/object/ui_object.h"

namespace seg {
namespace ui {
class ObjectInspectorWindow : public object::UIObject
{
   public:
    ObjectInspectorWindow();

    const std::string getType() const override { return "Object List Window"; }

   private:
    void drawImpl() override;

};  // class ObjectInspectorWindow
}  // namespace ui
}  // namespace seg

#endif