#ifndef SEG_OBJECT_BASIC_RENDERERS_H
#define SEG_OBJECT_BASIC_RENDERERS_H

#include <Eigen/Dense>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "seg/gl/shader.h"
#include "seg/object/gl_object.h"
#include "seg/types.h"

namespace seg {
namespace object {

class LineRenderer : public GLObject
{
   public:
    LineRenderer();

    const std::string getType() const override { return "Line"; }

    void setColor(const RGBA &_color) { color = _color; }

    void addData(const Eigen::Vector3f & vertex);
    void setData(std::vector<Eigen::Vector3f> && vertices);
    void setData(const std::vector<Eigen::Vector3f> & vertices);

    void setLineWidth(float _line_width) { line_width = _line_width; }
    const float &getLineWidth() { return line_width; }

   private:
    void drawImpl() override;

    RGBA color = RGBA(0.0f, 0.0f, 0.0f, 1.0f);
    float line_width = 2.0f;

    std::vector<Eigen::Vector3f> vertices;
};  // class LineRenderer

class StaticLineRenderer : public GLObject
{
   public:
    StaticLineRenderer(const std::tuple<std::vector<Eigen::Vector3f>,
                                        std::vector<Eigen::Vector3f>> &vertices_colors);
    StaticLineRenderer(
        const std::vector<Eigen::Vector3f> &vertices,
        const std::vector<Eigen::Vector3f> &colors = std::vector<Eigen::Vector3f>());

    void setColor(const RGBA &_color) { color = _color; }

    const std::string getType() const override { return "Static Line"; }

   private:
    void drawImpl() override;

    RGBA color = RGBA(0.0f, 0.0f, 0.0f, 1.0f);
    float line_width = 2.0f;

};  // class StaticLineRenderer

class PointcloudRenderer : public GLObject
{
    public:
    PointcloudRenderer();

    const std::string getType() const override { return "Pointcloud"; }
    void setColor(const RGBA &_color) { color = _color; }

    void addData(const Eigen::Vector3f &vertex);
    void setData(std::vector<Eigen::Vector3f> && vertices);
    void setData(const std::vector<Eigen::Vector3f> &vertices);

   private:
    void drawImpl() override;

    RGBA color = RGBA(0.0f, 0.0f, 0.0f, 1.0f);
    float point_size = 2.0f;

    std::vector<Eigen::Vector3f> vertices;
};  // class PoincloudRenderer

class StaticPointcloudRenderer : public GLObject
{
   public:
    StaticPointcloudRenderer(const std::vector<Eigen::Vector3f> &vertices,
                             const std::vector<float> &scalars);
    StaticPointcloudRenderer(
        const std::tuple<std::vector<Eigen::Vector3f>, std::vector<Eigen::Vector3f>>
            &vertices_colors);
    StaticPointcloudRenderer(
        const std::vector<Eigen::Vector3f> &vertices,
        const std::vector<Eigen::Vector3f> &colors = std::vector<Eigen::Vector3f>(),
        const std::vector<float> &scalars = std::vector<float>());
    void setColor(const RGBA &_color) { color = _color; }

    const std::string getType() const override { return "Static Pointcloud"; }

   private:
    void drawImpl() override;
    void drawInspector();

    gl::ColorMode color_mode = gl::ColorMode::UNIFORM;
    RGBA color = RGBA(0.0f, 0.0f, 0.0f, 1.0f);
    float point_size = 2.0f;

    float visualize_hue_from = 0.9f;
    float visualize_hue_to = 0.3f;
    float visualize_saturation = 0.7f;
    float visualize_value = 0.7f;

    float visualize_z_min = -10.0f;
    float visualize_z_max = 30.0f;

};  // class StaticPointcloudRenderer

class StaticMeshRenderer : public GLObject
{
   public:
    StaticMeshRenderer(
        const std::tuple<std::vector<Eigen::Vector3f>, std::vector<Triangle>>
            vertices_triangles);

    void setColor(const RGBA &_color) { color = _color; }
    const std::string getType() const override { return "Static Mesh"; }

   private:
    void drawImpl() override;

    RGBA color = RGBA(0.0f, 0.8f, 0.8f, 1.0f);

};  // class StaticMeshRenderer

}  // namespace object
}  // namespace seg

#endif