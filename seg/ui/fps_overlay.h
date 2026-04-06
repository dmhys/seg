#pragma once

#include <string>

#include "seg/object/ui_object.h"

namespace seg {
namespace ui {
class FpsOverlay : public object::UIObject {
 public:
  FpsOverlay();
  FpsOverlay(const FpsOverlay&) = delete;
  FpsOverlay& operator=(const FpsOverlay&) = delete;
  const std::string getType() const override { return "Fps Overlay"; }

 private:
  void drawImpl() override;

};  // class FpsOverlay
}  // namespace ui
}  // namespace seg
