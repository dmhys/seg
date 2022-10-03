#include "seg/object/gl_object.h"

#include "seg/gl/general_renderer.h"

namespace seg {
namespace object {
GLObject::GLObject() {}

// This allows forward declaration of GeneralRenderer in header files. 
// Do not remove.
GLObject::~GLObject() {} 

void GLObject::glFree()
{
    if(pimpl)
        pimpl->glFree();
}

}  // namespace object
}  // namespace seg