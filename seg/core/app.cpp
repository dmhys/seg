#include "seg/core/app.h"

#include <atomic>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "seg/core/config.h"
#include "seg/gl/scene.h"
#include "seg/core/object_manager.h"
#include "seg/types.h"
#include "seg/ui/controller.h"
#include "seg/resources/roboto_regular.h"
#include "seg/utilities/logger.h"

namespace {
void glfwErorrCallback(int error, const char* description) {
  LOG_ERROR("GLFW Error {} : {}", error, std::string(description));
}
}  // namespace

namespace seg {
App::~App() {
  turn_off_requested.store(true);
  if (seg_thread.joinable()) seg_thread.join();
}

void App::initialize(const std::string& window_name,
                     const WindowSize& window_size) {
  if (seg_thread.joinable())
    throw std::runtime_error("seg::initialize() called more than once");
  seg_thread = std::thread(&App::appMain, this, window_name, window_size);

  while (!initialized.load())
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  if (!window)
    throw std::runtime_error("SEG window creation failed");
}

void App::waitUntilClosed() {
  if (seg_thread.joinable()) seg_thread.join();
}

void App::appMain(const std::string& window_name,
                  const WindowSize& window_size) {
  windowSetup(window_name, window_size);
  if (!window) {
    initialized = true;
    return;
  }

  initializeComponents();

  initialized = true;

  while (!glfwWindowShouldClose(window) && !turn_off_requested.load()) {
    draw();
  }

  shutdown();
}

void App::windowSetup(const std::string& window_name,
                      const WindowSize& window_size) {
  // set error callback for glfw
  glfwSetErrorCallback(glfwErorrCallback);
  if (glfwInit() == GL_FALSE) throw std::runtime_error("GLFW init failed!");

  // GL 3.0 / glsl 330
  const char* glsl_version = "#version 330";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_SAMPLES, 4);

  // create window & context
  window = glfwCreateWindow(window_size.width, window_size.height,
                            window_name.c_str(), nullptr, nullptr);
  if (!window) {
    LOG_ERROR("Failed to create GLFW window");
    return;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // vsync

  // GLEW
  if (glewInit() != GLEW_OK) throw std::runtime_error("GLEW init failed!");
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.Fonts->AddFontFromMemoryCompressedTTF(
      RobotoRegular_compressed_data, RobotoRegular_compressed_size, 16.0f);
  if (getConfig().theme == Theme::LIGHT)
    ImGui::StyleColorsLight();
  else
    ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void App::initializeComponents() {
  if (controller == nullptr) throw std::runtime_error("Controller is not set!");
  if (object_manager == nullptr)
    throw std::runtime_error("Object manager is not set!");
  if (scene == nullptr) throw std::runtime_error("Scene is not set!");

  scene->init(window);
  object_manager->setShader(&scene->general_shader);
  controller->init(scene.get(), object_manager.get());
}

void App::draw() {
  // set frames
  glfwPollEvents();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  controller->drawUI();

  // set 3D viewport to dockspace central node area
  if (controller->viewport_size.width > 0 &&
      controller->viewport_size.height > 0) {
    int win_h;
    glfwGetWindowSize(window, nullptr, &win_h);
    glViewport(
        controller->viewport_pos.x,
        win_h - controller->viewport_pos.y - controller->viewport_size.height,
        controller->viewport_size.width, controller->viewport_size.height);
    scene->camera.onScreenResize(controller->viewport_size.width,
                                  controller->viewport_size.height);
  }

  scene->draw();
  object_manager->draw();

  // draw imgui render data onto gl buffer
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
}

void App::shutdown() {
  // Clean Up
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  scene->clearObjects();
  object_manager->onCoreShutdown();

  glfwDestroyWindow(window);
  window = nullptr;

  // glfwTerminate disabled — segfaults on WSL2 (Mesa TLS cleanup) and
  // native Linux Wayland (GLFW #2536, #1861, WSLg #715).
  // Root cause: Mesa/Wayland allocates TLS on the GL thread; cleanup
  // on thread exit dereferences freed Wayland proxies.
  // No upstream fix as of GLFW 3.4 / Mesa 24.x.
  // OS reclaims all resources on process exit.
  // glfwTerminate();
}

}  // namespace seg