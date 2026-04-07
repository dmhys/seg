#pragma once

#include "seg/types.h"

namespace seg {
enum class ThreadPolicy { MAIN_THREAD, OWN_THREAD };

enum class Verbosity {
  DEBUG = 1,
  INFO,
  WARN,
  ERROR,
  CRITICAL,
  QUIET,
};

enum class Theme {
  DARK,
  LIGHT,
};

typedef int LogFlag;
enum _LogFlag {
  LOG_NONE = 0,
  LOG_TERMINAL = 1 << 0,
  LOG_WINDOW = 1 << 1,
};

struct Options {
#ifdef __APPLE__
  const ThreadPolicy thread_policy = ThreadPolicy::MAIN_THREAD;
#else
  const ThreadPolicy thread_policy = ThreadPolicy::OWN_THREAD;
#endif
  Verbosity verbosity = Verbosity::INFO;
  Theme theme = Theme::LIGHT;
  LogFlag log_flag = 1;  // TODO: implement log flag configuration
};

}  // namespace seg
