#ifndef SEG_GL_SCENE_H
#define SEG_GL_SCENE_H

#include <memory>
#include <vector>

#include "seg/gl/camera.h"
#include "seg/gl/shader.h"
#include "seg/object/gl_object.h"

class GLFWwindow;

namespace seg {
namespace gl {
class Scene
{
   public:
    void init(GLFWwindow* window);
    void draw();

    void clearObjects();

    void onScreenResize(int width, int height);

    Camera camera;
    Shader shader;

   private:
    void clear();
    void updateVpMatrix();

    std::vector<std::unique_ptr<object::GLObject>> base_objects;

};  // class Scene
}  // namespace gl
}  // namespace seg

#endif