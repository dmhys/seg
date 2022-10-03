#ifndef SEG_OBJECT_GL_OBJECT_H
#define SEG_OBJECT_GL_OBJECT_H

#include <memory>
#include <Eigen/Dense>

#include "seg/object/object_base.h"

namespace seg {
namespace gl {
class Shader;
class GeneralRenderer;
}  // namespace gl

namespace object {
class GLObject : public ObjectBase
{
   public:
    GLObject();
    virtual ~GLObject();

    virtual void glFree();
    virtual void setShader(gl::Shader* _shader) { shader = _shader; }
    
    ObjectLayer getObjectLayer() override { return ObjectLayer::GL; }

   protected:
    gl::Shader* shader;  // TODO: derived classes shader null check
    Eigen::Matrix4f model_matrix = Eigen::Matrix4f::Identity();

    std::unique_ptr<gl::GeneralRenderer> pimpl;

};  // class GLObject
}  // namespace object
}  // namespace seg

#endif