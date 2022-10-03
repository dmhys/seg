#ifndef SEG_GL_GRID_RENDERER_H
#define SEG_GL_GRID_RENDERER_H

#include "seg/object/gl_object.h"
#include "seg/types.h"

typedef unsigned int GLuint;

namespace seg {
namespace gl {
class GridRenderer : public object::GLObject
{
   public:
    GridRenderer();
    ~GridRenderer();
    const std::string getType() const override { return "Grid"; }

   protected:
    void drawImpl() override;
    void findLod();

    RGBA color = RGBA(0.5f, 0.5f, 0.5f, 0.3f);
    const float line_width_thin = 0.5f;
    const float line_width_thick = 2.0f;

    unsigned int lod = 0;

    GLuint vao = 0;
    GLuint vbo[4];  // 0 - 100m, 1-20m, 2-5m, 3-1m;
    unsigned int vertex_count[4];

};  // class GridRenderer
}  // namespace gl
}  // namespace seg

#endif