#ifndef SEG_UI_FPS_OVERLAY_H
#define SEG_UI_FPS_OVERLAY_H

#include <string>

#include "seg/object/ui_object.h"

namespace seg {
namespace ui {
class FpsOverlay : public object::UIObject
{
   public:
    FpsOverlay();
    const std::string getType() const override { return "Fps Overlay"; }

   private:
    void drawImpl() override;

};  // class FpsOverlay
}  // namespace ui
}  // namespace seg

#endif