#pragma once

#include <string>

#include "seg/object/ui_object.h"

namespace seg {
namespace object {
class ObjectManager;
}

namespace ui {
class ObjectInspectorWindow : public object::UIObject {
 public:
  ObjectInspectorWindow(object::ObjectManager* om);
  ObjectInspectorWindow(const ObjectInspectorWindow&) = delete;
  ObjectInspectorWindow& operator=(const ObjectInspectorWindow&) = delete;

  const std::string getType() const override {
    return "Object Inspector Window";
  }

 private:
  void drawImpl() override;
  object::ObjectManager* object_manager;

};  // class ObjectInspectorWindow
}  // namespace ui
}  // namespace seg
