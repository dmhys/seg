#pragma once

#include "seg/object/object_base.h"

typedef int ImGuiWindowFlags;

namespace seg {
namespace object {
class UIObject : public ObjectBase {
 public:
  virtual ~UIObject() {}
  ObjectLayer getObjectLayer() const override { return ObjectLayer::UI; }

 protected:
  ImGuiWindowFlags window_flag = 0;
};

}  // namespace object
}  // namespace seg
