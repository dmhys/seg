#pragma once

#include "seg/object/gl_object.h"
#include "seg/types.h"

typedef unsigned int GLuint;

namespace seg {
namespace gl {
class GridRenderer : public object::GLObject {
 public:
  GridRenderer();
  ~GridRenderer();
  const std::string getType() const override { return "Grid"; }

 private:
  void drawImpl() override;

  RGBA grid_color = RGBA(0.5f, 0.5f, 0.5f, 0.5f);
  float fade_distance = 200.0f;

  GLuint vao = 0;

};  // class GridRenderer
}  // namespace gl
}  // namespace seg
