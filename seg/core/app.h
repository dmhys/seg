#ifndef SEG_CORE_APP_H
#define SEG_CORE_APP_H

#include <atomic>
#include <memory>
#include <thread>

#include "seg/types.h"

// forward declarations
class GLFWwindow;

// ======================================================================
namespace seg {
namespace gl {
class Scene;
}
namespace ui {
class Controller;
}
namespace object {
class ObjectManager;
}

class App
{
   public:
    App(){};
    ~App();
    void initialize(const std::string& window_name, const WindowSize& window_size);

    void setController(ui::Controller* _controller) { controller.reset(_controller); }
    void setObjectManager(object::ObjectManager* _object_manager)
    {
        object_manager.reset(_object_manager);
    }
    void setScene(gl::Scene* _scene) { scene.reset(_scene); }

   protected:
    GLFWwindow* window;

    std::unique_ptr<ui::Controller> controller;
    std::unique_ptr<object::ObjectManager> object_manager;
    std::unique_ptr<gl::Scene> scene;

    std::thread seg_thread;
    bool turn_off_requseted = false;
    std::atomic<bool> initialized{false};

    void appMain(const std::string& window_name, const WindowSize& window_size);

    void windowSetup(const std::string& window_name, const WindowSize& window_size);
    void initializeComponents();
    void draw();
    void shutdown();

};  // class App
}  // namespace seg

#endif