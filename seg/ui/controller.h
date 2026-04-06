#pragma once

#include <memory>
#include <vector>

#include "seg/object/ui_object.h"
#include "seg/types.h"

namespace seg {
namespace gl {
class Scene;
};
namespace object {
class ObjectManager;
};

namespace ui {
class Controller {
 public:
  Controller() = default;
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

  void init(gl::Scene* scene, object::ObjectManager* object_manager);
  void drawUI();

  Position viewport_pos;
  WindowSize viewport_size;

 private:
  void handleMouseEvents();

  gl::Scene* scene;
  object::ObjectManager* object_manager;

  std::vector<std::unique_ptr<object::UIObject>> base_objects;

};  // class Controller
}  // namespace ui
}  // namespace seg
