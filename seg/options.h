#ifndef SEG_OPTIONS_H
#define SEG_OPTIONS_H

#include "seg/types.h"

namespace seg {
enum class Verbosity
{
    DEBUG = 1,
    INFO,
    WARN,
    ERROR,
    CRITICAL,
    QUIET,
};

enum class Theme
{
    DARK,
    LIGHT,
};

typedef int LogFlag;
enum _LogFlag
{
    LOG_NONE = 0,
    LOG_TERMINAL = 1 << 0,
    LOG_WINDOW = 1 << 1,
};

struct Options
{
    Verbosity verbosity;
    Theme theme;
    LogFlag log_flag = 1;  // TODO:
    Options() : verbosity(Verbosity::INFO), theme(Theme::LIGHT) {}
};

}  // namespace seg

#endif