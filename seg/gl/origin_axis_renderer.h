#pragma once

#include "seg/object/gl_object.h"

typedef unsigned int GLuint;

namespace seg {
namespace gl {
class OriginAxisRenderer : public object::GLObject {
 public:
  OriginAxisRenderer();
  ~OriginAxisRenderer();
  const std::string getType() const override { return "Origin Axis"; }

 private:
  void drawImpl() override;

};  // class OriginAxisRenderer
}  // namespace gl
}  // namespace seg
