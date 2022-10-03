#ifndef SEG_CORE_CONFIG_H
#define SEG_CORE_CONFIG_H

#include <string>

#include "seg/options.h"
#include "seg/types.h"

namespace seg {
namespace object {
class ObjectBase;
}

namespace core {
class Config
{
   public:
    static Config& getInstance();

    Theme theme = Theme::LIGHT;
    float camera_distance;  // meter

    bool show_fps = true;
    bool show_grid = true;
    bool show_origin = true;

    std::string selected_object_name = "";
    object::ObjectBase* selected_object = nullptr;

   private:
    Config(){};

};  // class Config
}  // namespace core

inline core::Config& getConfig()  // shortand
{
    return core::Config::getInstance();
}

// ====================================================================

static const RGBA COLORTHEME_DARK_BG_COLOR{0.2f, 0.2f, 0.2f, 1.0f};
static const RGBA COLORTHEME_LIGHT_BG_COLOR{0.9f, 0.9f, 0.9f, 1.0f};
static const RGBA* theme_value_ptr[] = {&COLORTHEME_DARK_BG_COLOR, &COLORTHEME_LIGHT_BG_COLOR};

inline const RGBA enumToStruct(Theme theme) { return *(theme_value_ptr[static_cast<int>(theme)]); }

}  // namespace seg

#endif