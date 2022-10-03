#include "seg/core/config.h"

#include <memory>
#include <mutex>

static std::once_flag instance_flag;
static std::unique_ptr<seg::core::Config> config;

namespace seg {
namespace core {
Config& Config::getInstance()
{
    std::call_once(instance_flag, [] { config.reset(new Config()); });

    return *(config.get());
}

}  // namespace core
}  // namespace seg