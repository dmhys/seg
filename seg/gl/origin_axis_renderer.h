#ifndef SEG_GL_ORIGIN_AXIS_RENDERER_H
#define SEG_GL_ORIGIN_AXIS_RENDERER_H

#include "seg/object/gl_object.h"

typedef unsigned int GLuint;

namespace seg {
namespace gl {
class OriginAxisRenderer : public object::GLObject
{
   public:
    OriginAxisRenderer();
    ~OriginAxisRenderer();
    const std::string getType() const override { return "Origin Axis"; }

   protected:
    void drawImpl() override;

};  // class OriginAxisRenderer
}  // namespace gl
}  // namespace seg

#endif