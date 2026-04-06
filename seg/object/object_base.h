#pragma once

#include <memory>
#include <string>

namespace seg {
namespace ui {
class GeneralInspector;
}

namespace object {
enum ObjectLayer {
  GL,
  UI,
};

class ObjectBase {
 public:
  ObjectBase();
  virtual ~ObjectBase();
  void draw() {
    if (is_visible) drawImpl();
  }
  virtual ObjectLayer getObjectLayer() const = 0;
  virtual const std::string getType() const = 0;

  bool is_visible = true;
  std::unique_ptr<ui::GeneralInspector> inspector;

 private:
  // Uncopyable
  ObjectBase(const ObjectBase&) = delete;
  ObjectBase& operator=(const ObjectBase&) = delete;

  virtual void drawImpl() = 0;

};  // class ObjectBase
}  // namespace object
}  // namespace seg
