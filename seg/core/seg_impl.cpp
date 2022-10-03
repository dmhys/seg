#include <iostream>
#include <memory>
#include <mutex>

#include "assert.h"
#include "seg/core/app.h"
#include "seg/core/config.h"
#include "seg/gl/scene.h"
#include "seg/object/gl/basic_renderers.h"
#include "seg/object/object_manager.h"
#include "seg/object/primitives.h"
#include "seg/options.h"
#include "seg/seg.h"
#include "seg/types.h"
#include "seg/ui/controller.h"
#include "seg/utilities/logger.h"

static std::once_flag instance_flag;
static std::unique_ptr<seg::App> app;

seg::object::ObjectManager* object_manager = nullptr;

namespace seg {
// internal Functions ===================================================
void createInstances()
{
    // note - ownership of each modules belong to App()
    std::call_once(instance_flag, [] {
        app.reset(new App());
        object_manager = new object::ObjectManager();

        app->setController(new ui::Controller());
        app->setObjectManager(object_manager);
        app->setScene(new gl::Scene());
    });
}

void setOptions(Options options)
{
#ifdef SPDLOG
    spdlog::set_pattern("\033[1;94m[SEG]\033[0m[%t][%H:%M:%S][%^%l%$] %v");
#endif
    SET_LOGGER_LEVEL(options.verbosity);

    getConfig().theme = options.theme;
}

void initializeApp(const std::string& window_name, const WindowSize& window_size)
{
    LOG_INFO("Initializing . . .");

    app->initialize(window_name, window_size);

    LOG_INFO("Initialized!");
}

// APIs =================================================================
void initialize(const std::string& window_name, const WindowSize& window_size,
                Options options)
{
    createInstances();
    setOptions(options);
    initializeApp(window_name, window_size);
}

void addObject(const std::string& name, const std::shared_ptr<object::ObjectBase>& object)
{
    object_manager->addObject(name, object);
}

void addObject(const std::string& name, object::ObjectBase* object)
{
    object_manager->addObject(name, object);
}

std::string addObject(const std::shared_ptr<object::ObjectBase>& object)
{
    return object_manager->addObject(object);
}

std::string addObject(object::ObjectBase* object)
{
    return object_manager->addObject(object);
}

bool deleteObject(const std::string & name)
{
    return object_manager->deleteObject(name);
}

}  // namespace seg