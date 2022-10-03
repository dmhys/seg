#include "seg/gl/general_renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <mutex>
#include <stdexcept>

#include "seg/gl/shader.h"
#include "seg/utilities/logger.h"

namespace seg {
namespace gl {
GeneralRenderer::~GeneralRenderer()
{
    glFree();
}

void GeneralRenderer::draw(Shader* shader)
{
    if (updated.load() == true) {
        glMalloc();
        updated.store(false);
    }

    glBindVertexArray(vao);

    const GLuint vertex_id = shader->getAttribId("vertex_pos_model");
    glEnableVertexAttribArray(vertex_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(vertex_id, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    if (cbo != 0) {
        const GLuint color_id = shader->getAttribId("vertex_color_rgb");
        glEnableVertexAttribArray(color_id);
        glBindBuffer(GL_ARRAY_BUFFER, cbo);
        glVertexAttribPointer(color_id, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    if (sbo != 0) {
        const GLuint intensity_id = shader->getAttribId("vertex_intensity");
        glEnableVertexAttribArray(intensity_id);
        glBindBuffer(GL_ARRAY_BUFFER, sbo);
        glVertexAttribPointer(intensity_id, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    if (eao == 0)  // draw array
        glDrawArrays(static_cast<int>(render_target), 0, vertex_count);
    else  // draw elements
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao);
        glDrawElements(static_cast<int>(render_target), index_count, GL_UNSIGNED_INT,
                       (void*)0);
    }
}

void GeneralRenderer::glMalloc()
{
    if (buff_generated == false) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);

        buff_generated = true;
    }

    auto conditional_lock = (buffer_type == BufferType::STATIC)
                                ? std::unique_lock<std::mutex>()
                                : std::unique_lock<std::mutex>(mtx);

    vertex_count = tmp_vertices.size();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count * 3, tmp_vertices.data(),
                 static_cast<int>(buffer_type));
    tmp_vertices.clear();

    if (tmp_colors.empty() == false)  // not empty
    {
        if (cbo == 0) glGenBuffers(1, &cbo);

        glBindBuffer(GL_ARRAY_BUFFER, cbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count * 3, tmp_colors.data(),
                     static_cast<int>(buffer_type));
        tmp_colors.clear();
    }

    if (tmp_scalars.empty() == false) {
        if (sbo == 0) glGenBuffers(1, &sbo);

        glBindBuffer(GL_ARRAY_BUFFER, sbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count, tmp_scalars.data(),
                     static_cast<int>(buffer_type));
        tmp_scalars.clear();
    }

    if (tmp_indices.empty() == false) {
        if (eao == 0) glGenBuffers(1, &eao);

        index_count = tmp_indices.size() * 3;
        glBindBuffer(GL_ARRAY_BUFFER, eao);
        glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * index_count,
                     tmp_indices.data(), static_cast<int>(buffer_type));
        tmp_indices.clear();
    }
}

void GeneralRenderer::glFree()
{
    if (buff_generated == false) return;

    glDeleteBuffers(1, &vbo);

    if (cbo != 0) glDeleteBuffers(1, &cbo);
    if (sbo != 0) glDeleteBuffers(1, &sbo);
    if (sbo != 0) glDeleteBuffers(1, &eao);

    glDeleteVertexArrays(1, &vao);

    buff_generated = false;
}

void GeneralRenderer::setData(std::vector<Eigen::Vector3f>&& vertices,
                                 std::vector<Eigen::Vector3f>&& colors,
                                 std::vector<float>&& scalars,
                                 std::vector<Triangle>&& indices)
{
    std::lock_guard<std::mutex> lock(mtx);

    const size_t new_vertex_count = vertices.size();
    const bool maintain_validity = (new_vertex_count == vertex_count);

    tmp_vertices = std::move(vertices);
    vertex_count = new_vertex_count;

    if (colors.size() == vertex_count) {
        tmp_colors = std::move(colors);
        has_valid_color = true;
    } else
        has_valid_color = has_valid_color && maintain_validity;

    if (scalars.size() == vertex_count) {
        tmp_scalars = std::move(scalars);
        has_valid_scalar = true;
    } else
        has_valid_scalar = has_valid_scalar && maintain_validity;

    // indices validity check
    bool valid_index = true;
    for (const auto& triangle : indices) {
        const bool current_triangle_valid = (triangle.vertex_index[0] < vertex_count) &&
                                            (triangle.vertex_index[1] < vertex_count) &&
                                            (triangle.vertex_index[2] < vertex_count);

        if (current_triangle_valid == false) {
            valid_index = false;
            LOG_FATAL("Renderer - Invalid index array!");
            throw std::invalid_argument("Invalid index array given!");
            break;
        }
    }

    if (valid_index) tmp_indices = move(indices);

    updated.store(true);
}

void GeneralRenderer::setData(std::vector<Eigen::Vector3f>&& vertices,
                                 std::vector<float>&& scalars)
{
    setData(std::move(vertices), std::vector<Eigen::Vector3f>(), std::move(scalars));
}

void GeneralRenderer::setData(std::vector<Eigen::Vector3f>&& vertices,
                                 std::vector<Triangle>&& indices)
{
    setData(std::move(vertices), std::vector<Eigen::Vector3f>(), std::vector<float>(),
               std::move(indices));
}

void GeneralRenderer::setData(std::vector<Eigen::Vector3f>&& vertices,
                                 std::vector<Eigen::Vector3f>&& colors,
                                 std::vector<Triangle>&& indices)
{
    setData(std::move(vertices), std::move(colors), std::vector<float>(),
               std::move(indices));
}

}  // namespace gl
}  // namespace seg