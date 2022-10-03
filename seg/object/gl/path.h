#ifndef SEG_OBJECT_PATH_H
#define SEG_OBJECT_PATH_H

#include <mutex>
#include <Eigen/Dense>
#include <vector>

#include "seg/object/gl_object.h"
#include "seg/types.h"

namespace seg {
namespace gl {
class Shader;
}

namespace object {

class LineRenderer;
class Pose;

class Path : public GLObject
{
   public:
    enum class VisualType
    {
        LINE = 1 << 0,
        AXIS = 1 << 1,
        CAMERA_FRAMES = 1 << 2,

        AXIS_WITH_LINE = (1 << 1) + (1 << 0),
        CAMERA_FRAMES_WITH_LINE = (1 << 2) + (1 << 0),
    };

   public:
    Path();
    Path(std::vector<Eigen::Matrix4f> poses);
    const std::string getType() const override { return "Path"; }

    void glFree() override;
    void setShader(gl::Shader* shader) override;

    void setCameraParameters(float width, float height, float focal_length);
    void setCameraParameters(float width, float height, float fx, float fy, float cx,
                             float cy);

    void reset();
    void reset(const std::vector<Eigen::Matrix4f> & poses);
    void reset(std::vector<Eigen::Matrix4f> && poses);
    
    void addPose(const Eigen::Matrix4f & pose);

    void setVisualType(VisualType _type) { type = _type; }

private:
    void drawImpl() override;
    void drawInspector();

    VisualType type = VisualType::LINE;

    RGBA frame_color = RGBA(0.0f, 0.0f, 0.0f, 1.0f);
    RGBA line_color = RGBA(0.0f, 1.0f, 0.0f, 1.0f);

    float axis_frame_line_width = 3.0f;
    float line_width = 1.0f;
    
    std::mutex pose_mtx;
    size_t num_pose;
    std::vector<Eigen::Matrix4f> poses;

    std::unique_ptr<LineRenderer> line_renderer;
    std::unique_ptr<Pose> pose_renderer;
};  // class Path

}  // namespace object

const char* enumToCharP(object::Path::VisualType type);

}  // namespace seg

#endif