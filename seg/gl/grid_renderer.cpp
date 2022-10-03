#include "seg/gl/grid_renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Eigen/Dense>
#include <vector>

#include "seg/core/config.h"
#include "seg/gl/shader.h"
#include "seg/object/gl_object.h"
#include "seg/object/primitives.h"
#include "seg/options.h"
#include "seg/types.h"
#include "seg/utilities/logger.h"

namespace seg {
namespace gl {
GridRenderer::GridRenderer()
{
    auto lod0_vertices = object::primitives::Grid(10000, 100.0f);
    auto lod1_vertices = object::primitives::Grid(10000, 100.0f);
    auto lod2_vertices = object::primitives::Grid(5000, 10.0f);
    auto lod3_vertices = object::primitives::Grid(300, 1.0f);

    // vao, vbo gen
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(4, &vbo[0]);

    // lod0 - 100m
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    vertex_count[0] = lod0_vertices.size();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count[0] * 3, lod0_vertices.data(),
                 GL_STATIC_DRAW);

    // lod1 - 20m
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    vertex_count[1] = lod1_vertices.size();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count[1] * 3, lod1_vertices.data(),
                 GL_STATIC_DRAW);

    // lod2 - 5m
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    vertex_count[2] = lod2_vertices.size();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count[2] * 3, lod2_vertices.data(),
                 GL_STATIC_DRAW);

    // lod3 - 1m
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    vertex_count[3] = lod3_vertices.size();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_count[3] * 3, lod3_vertices.data(),
                 GL_STATIC_DRAW);
}

GridRenderer::~GridRenderer()
{
    glDeleteBuffers(4, &vbo[0]);
    glDeleteVertexArrays(1, &vao);
}

void GridRenderer::drawImpl()
{
    if (getConfig().show_grid == false) return;

    shader->bind();
    shader->setModelMatrix(model_matrix);
    shader->setColorMode(gl::ColorMode::UNIFORM);
    shader->setUniform("uniform_color", color.asEigenVector4f());

    findLod();

    glBindVertexArray(vao);

    GLuint vertex_id = shader->getAttribId("vertex_pos_model");
    glEnableVertexAttribArray(vertex_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[lod]);
    glVertexAttribPointer(vertex_id, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glLineWidth(line_width_thin);
    glDrawArrays(GL_LINES, 0, vertex_count[lod]);

    glEnableVertexAttribArray(vertex_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[lod - 1]);
    glVertexAttribPointer(vertex_id, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glLineWidth(line_width_thick);
    glDrawArrays(GL_LINES, 0, vertex_count[lod - 1]);
}

void GridRenderer::findLod()
{
    const float dist = getConfig().camera_distance;
    if (dist < 70)
        lod = 3;
    else if (dist < 700)
        lod = 2;
    else
        lod = 1;
}

}  // namespace gl
}  // namespace seg