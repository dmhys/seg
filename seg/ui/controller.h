#ifndef SEG_UI_CONTROLLER_H
#define SEG_UI_CONTROLLER_H

#include <memory>
#include <vector>

#include "seg/object/ui_object.h"

namespace seg {
namespace gl {
class Scene;
};
namespace object {
class ObjectManager;
};

namespace ui {
class Controller
{
   public:
    void init(gl::Scene* scene, object::ObjectManager* object_manager);
    void drawUI();

   private:
    void handleMouseEvents();

    gl::Scene* scene;
    object::ObjectManager* object_manager;

    std::vector<std::unique_ptr<object::UIObject>> base_objects;

};  // class Controller
}  // namespace ui
}  // namespace seg

#endif