#ifndef SEG_GL_GENERAL_RENDERER_H
#define SEG_GL_GENERAL_RENDERER_H

#include <GL/glew.h>

#include <Eigen/Dense>
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include "seg/types.h"  // seg::Triangle

typedef unsigned int GLuint;

namespace seg {
namespace gl {
class Shader;

class GeneralRenderer
{
   public:
    enum class BufferType
    {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_STREAM_DRAW,
    };

    enum class RenderTarget
    {
        POINT = GL_POINTS,
        LINE = GL_LINES,
        LINE_STRIP = GL_LINE_STRIP,
        TRIANGLES = GL_TRIANGLES,
    };

   public:
    GeneralRenderer(BufferType _buffer_type, RenderTarget _render_target)
        : buffer_type(_buffer_type), render_target(_render_target)
    {
    }
    ~GeneralRenderer();

    void glFree();

    void draw(Shader* shader);

    const bool & hasColor() { return has_valid_color; }
    const bool & hasScalar() { return has_valid_scalar; }

    const size_t& vertexCount() const { return vertex_count; }

   private:
    void glMalloc();

    const BufferType buffer_type;
    const RenderTarget render_target;
    size_t vertex_count;
    size_t index_count;

    std::atomic<bool> updated{false};
    std::mutex mtx;
    std::vector<Eigen::Vector3f> tmp_vertices;
    std::vector<Eigen::Vector3f> tmp_colors;
    std::vector<float> tmp_scalars;
    std::vector<Triangle> tmp_indices;

    bool buff_generated = false;

    bool has_valid_color = false;
    bool has_valid_scalar = false;

    GLuint vao = 0;
    GLuint vbo = 0;  // vertex buffer
    GLuint cbo = 0;  // color buffer
    GLuint sbo = 0;  // scalar buffer
    GLuint eao = 0;  // element array

   public:
    void setData(std::vector<Eigen::Vector3f>&& vertices,
                    std::vector<Eigen::Vector3f>&& colors = std::vector<Eigen::Vector3f>(),
                    std::vector<float>&& scalars = std::vector<float>(),
                    std::vector<Triangle>&& indices = std::vector<Triangle>());

    void setData(std::vector<Eigen::Vector3f>&& vertices, std::vector<float>&& scalars);
    void setData(std::vector<Eigen::Vector3f>&& vertices, std::vector<Triangle>&& indices);

    void setData(std::vector<Eigen::Vector3f>&& vertices, std::vector<Eigen::Vector3f>&& colors,
                    std::vector<Triangle>&& indices);
};  // class GeneralRenderer
}  // namespace gl
}  // namespace seg

#endif