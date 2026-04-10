#include "seg/internal/logger.h"

#include <memory>
#include <mutex>

#ifdef USE_SPDLOG
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace {
std::once_flag flag;
std::unique_ptr<spdlog::logger> instance;
}  // namespace

namespace seg {
spdlog::logger* Logger::getInstance() {
  std::call_once(flag, [] {
    auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    instance = std::make_unique<spdlog::logger>("seg", sink);

#ifdef DEBUG
    instance->set_pattern(
        "[%H:%M:%S] [Thread: %t] [%@]\n"
        "[%^%l%$] %v");
#else
    instance->set_pattern("\033[1;96m[SEG]\033[0m[%H:%M:%S][%^%l%$] %v");
#endif

    instance->set_level(spdlog::level::trace);
  });
  return instance.get();
}
}  // namespace seg
#endif
