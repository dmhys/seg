#pragma once

#include <atomic>
#include <memory>
#include <thread>

#include "seg/options.h"
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

class App {
 public:
  App() {};
  ~App();
  void initialize(const std::string& window_name,
                  const WindowSize& window_size,
                  ThreadPolicy thread_policy);
  void appMain();
  void waitUntilClosed();
  bool isRunning() const { return running.load(); }

  void setController(ui::Controller* _controller) {
    controller.reset(_controller);
  }
  void setObjectManager(object::ObjectManager* _object_manager) {
    object_manager.reset(_object_manager);
  }
  void setScene(gl::Scene* _scene) { scene.reset(_scene); }

 private:
  GLFWwindow* window = nullptr;

  std::unique_ptr<ui::Controller> controller;
  std::unique_ptr<object::ObjectManager> object_manager;
  std::unique_ptr<gl::Scene> scene;

  ThreadPolicy thread_policy;
  std::string window_name;
  WindowSize window_size;

  std::thread seg_thread;
  std::atomic<bool> turn_off_requested{false};
  std::atomic<bool> initialized{false};
  std::atomic<bool> running{false};

  void windowSetup();
  void initializeComponents();
  void draw();
  void shutdown();

};  // class App
}  // namespace seg
