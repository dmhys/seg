#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>

#include "seg/core/app.h"
#include "seg/core/config.h"
#include "seg/core/object_manager.h"
#include "seg/gl/scene.h"
#include "seg/object/gl/basic_renderers.h"
#include "seg/object/primitives.h"
#include "seg/options.h"
#include "seg/seg.h"
#include "seg/types.h"
#include "seg/ui/controller.h"
#include "seg/internal/logger.h"

namespace {
std::once_flag instance_flag;
std::unique_ptr<seg::App> app;
seg::object::ObjectManager* object_manager = nullptr;

void ensureInitialized() {
  if (object_manager == nullptr)
    throw std::runtime_error("seg::initialize() must be called first");
}

}  // namespace

namespace seg {
namespace {
// internal Functions ===================================================
void createInstances() {
  // note - ownership of each modules belong to App()
  std::call_once(instance_flag, [] {
    app.reset(new App());
    object_manager = new object::ObjectManager();

    app->setController(new ui::Controller());
    app->setObjectManager(object_manager);
    app->setScene(new gl::Scene());
  });
}

void setOptions(Options options) {
  SET_LOG_LEVEL(static_cast<int>(options.verbosity));
  getConfig().theme = options.theme;
}

void initializeApp(const std::string& window_name,
                   const WindowSize& window_size,
                   ThreadPolicy thread_policy) {
  LOG_INFO("{}", std::string(80, '='));
  LOG_INFO("Initializing 🛞 SEG . . .");

  app->initialize(window_name, window_size, thread_policy);

  LOG_INFO("🛞 SEG good to go!");
  LOG_INFO("{}", std::string(80, '='));
}
}  // namespace

// APIs =================================================================
void initialize(const std::string& window_name,
                const WindowSize& window_size,
                Options options) {
#ifdef __APPLE__
  if (options.thread_policy == ThreadPolicy::OWN_THREAD)
    throw std::runtime_error(
        "OWN_THREAD is not supported on macOS. "
        "macOS requires rendering on the main thread.");
#endif

  createInstances();
  setOptions(options);
  initializeApp(window_name, window_size, options.thread_policy);
}

void run() {
  ensureInitialized();

  if (app->isRunning())
    throw std::runtime_error("seg::run() called while already running");

  app->appMain();
}

void addObject(const std::string& name,
               const std::shared_ptr<object::ObjectBase>& object) {
  ensureInitialized();

  object_manager->addObject(name, object);
}

void addObject(const std::string& name, object::ObjectBase* object) {
  addObject(name, std::shared_ptr<object::ObjectBase>(object));
}

std::string addObject(const std::shared_ptr<object::ObjectBase>& object) {
  ensureInitialized();

  return object_manager->addObject(object);
}

std::string addObject(object::ObjectBase* object) {
  return addObject(std::shared_ptr<object::ObjectBase>(object));
}

bool deleteObject(const std::string& name) {
  ensureInitialized();

  return object_manager->deleteObject(name);
}

void shutdown() {
  ensureInitialized();

  app->requestShutdown();
}

void waitUntilClosed() {
  ensureInitialized();

  app->waitUntilClosed();
}

}  // namespace seg