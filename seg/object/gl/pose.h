#ifndef SEG_OBJECT_POSE_H
#define SEG_OBJECT_POSE_H

#include <Eigen/Dense>
#include <mutex>
#include <tuple>
#include <vector>

#include "seg/object/gl_object.h"
#include "seg/types.h"

namespace seg {
namespace object {
class Pose : public GLObject
{
   public:
    enum class VisualType
    {
        AXIS,
        CAMERA_FRAME,
    };

   public:
    Pose();
    Pose(const Eigen::Matrix4f pose);
    const std::string getType() const override { return "Pose"; }

    void setCameraParameters(float width, float height, float focal_length);
    void setCameraParameters(float width, float height, float fx, float fy, float cx,
                             float cy);
    void setColor(const RGBA& _color) { color = _color; }
    void setScale(float _scale){scale = _scale;};
    void setLineWidth(float _line_width) { line_width = _line_width; }

    void setData(Eigen::Matrix4f&& pose);
    void setData(const Eigen::Matrix4f& pose);

    void setVisualType(VisualType _type) { type = _type; }

    float line_width = 2.0f;
    float scale = 0.3f;

   private:
    void drawImpl() override;
    void drawInspector();

    RGBA color = RGBA(0.0f, 0.0f, 0.0f, 1.0f);

    Eigen::Matrix4f pose;
    VisualType type = VisualType::AXIS;
    VisualType prev = VisualType::AXIS;

    std::mutex pose_mtx;
    std::vector<Eigen::Vector3f> camera_frame;
    std::tuple<std::vector<Eigen::Vector3f>, std::vector<Eigen::Vector3f>> axis;

};  // class Path

}  // namespace object

const char* enumToCharP(object::Pose::VisualType type);

}  // namespace seg

#endif