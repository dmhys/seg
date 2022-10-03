#include "seg/object/primitives.h"

#include <Eigen/Dense>
#include <algorithm>  // std max & min, swap
#include <cmath>
#include <map>
#include <random>
#include <stdexcept>  // invalid argument
#include <string>     // error msg
#include <vector>

#include "seg/types.h"
#include "seg/utilities/logger.h"

namespace icosahedron {
const float x = .525731112119133606f;
const float z = .850650808352039932f;
const float l = 1.05146222423826721f;  // default length of each edge

using vec3 = Eigen::Vector3f;
static const std::vector<Eigen::Vector3f> vertices =  // 12 vertices
    {vec3(-x, 0, z), vec3(x, 0, z),  vec3(-x, 0, -z), vec3(x, 0, -z),
     vec3(0, z, x),  vec3(0, z, -x), vec3(0, -z, x),  vec3(0, -z, -x),
     vec3(z, x, 0),  vec3(-z, x, 0), vec3(z, -x, 0),  vec3(-z, -x, 0)};

static const std::vector<seg::Triangle> triangles =  // 20 faces
    {{0, 4, 1}, {0, 9, 4},  {9, 5, 4},  {4, 5, 8},  {4, 8, 1},  {8, 10, 1}, {8, 3, 10},
     {5, 3, 8}, {5, 2, 3},  {2, 7, 3},  {7, 10, 3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6},
     {0, 1, 6}, {6, 1, 10}, {9, 0, 11}, {9, 11, 2}, {9, 2, 5},  {7, 2, 11}};
}  // namespace icosahedron

namespace cameraframe {
static const std::vector<seg::Edge> edges =  // 8 lines
    {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2}, {2, 4}, {3, 4}, {1, 3}};

std::vector<Eigen::Vector3f> generate(float left, float right, float upper, float lower,
                                      float scale)
{
    if (right < left) std::swap(right, left);
    if (lower < upper) std::swap(lower, upper);

    std::vector<Eigen::Vector3f> vertices;
    vertices.reserve(5);
    vertices.push_back(Eigen::Vector3f(0, 0, 0));
    vertices.push_back(scale * Eigen::Vector3f(left, upper, 1.0f));
    vertices.push_back(scale * Eigen::Vector3f(right, upper, 1.0f));
    vertices.push_back(scale * Eigen::Vector3f(left, lower, 1.0f));
    vertices.push_back(scale * Eigen::Vector3f(right, lower, 1.0f));

    std::vector<Eigen::Vector3f> return_value;
    return_value.reserve(16);
    for (auto &&edge : edges) {
        return_value.push_back(vertices[edge.vertex_index[0]]);
        return_value.push_back(vertices[edge.vertex_index[1]]);
    }

    return return_value;
}
}  // namespace cameraframe

namespace seg {
namespace object {

std::vector<Eigen::Vector3f> primitives::Grid(Eigen::Vector2f max, Eigen::Vector2f min,
                                              Eigen::Vector2f cell_size)
{
    // Exception Handle
    if (max.x() < 0 || max.y() < 0 || 0 < min.x() || 0 < min.y())
        throw std::invalid_argument("primitives::Grid - Invlalid max/min boundary.");

    if (cell_size.x() < 0 || cell_size.y() < 0)
        throw std::invalid_argument("primitives::Grid - given cell size is negative.");

    if (max.x() < cell_size.x() || max.y() < cell_size.y() || -cell_size.x() < min.x() ||
        -cell_size.y() < min.y())
        throw std::invalid_argument(
            "primitives::Grid - requested grid is too small for cell.");

    const Eigen::Vector2i max_uv(static_cast<int>(max.x() / cell_size.x()),
                                 static_cast<int>(max.y() / cell_size.y()));
    const Eigen::Vector2i min_uv(static_cast<int>(min.x() / cell_size.x()),
                                 static_cast<int>(min.y() / cell_size.y()));
    const Eigen::Vector2f max_xy(max_uv.x() * cell_size.x(), max_uv.y() * cell_size.y());
    const Eigen::Vector2f min_xy(min_uv.x() * cell_size.x(), min_uv.y() * cell_size.y());

    std::vector<Eigen::Vector3f> grid;
    grid.reserve((max_uv.x() - min_uv.x() + 1) * 2 + (max_uv.y() - min_uv.y() + 1) * 2);

    // x grid
    for (int i = min_uv.x(); i <= max_uv.x(); i++) {
        grid.push_back(Eigen::Vector3f(i * cell_size.x(), min_xy.y(), 0.0f));
        grid.push_back(Eigen::Vector3f(i * cell_size.x(), max_xy.y(), 0.0f));
    }

    // y grid
    for (int i = min_uv.y(); i <= max_uv.y(); i++) {
        grid.push_back(Eigen::Vector3f(min_xy.x(), i * cell_size.y(), 0.0f));
        grid.push_back(Eigen::Vector3f(max_xy.x(), i * cell_size.y(), 0.0f));
    }

    return grid;
}

std::tuple<std::vector<Eigen::Vector3f>, std::vector<Eigen::Vector3f>> primitives::Axis(
    float axis_length)
{
    std::vector<Eigen::Vector3f> vertices;
    std::vector<Eigen::Vector3f> colors;
    vertices.reserve(6);
    colors.reserve(6);

    vertices.push_back(Eigen::Vector3f(0.0f, 0.0f, 0.0f));
    vertices.push_back(Eigen::Vector3f(axis_length, 0.0f, 0.0f));
    vertices.push_back(Eigen::Vector3f(0.0f, 0.0f, 0.0f));
    vertices.push_back(Eigen::Vector3f(0.0f, axis_length, 0.0f));
    vertices.push_back(Eigen::Vector3f(0.0f, 0.0f, 0.0f));
    vertices.push_back(Eigen::Vector3f(0.0f, 0.0f, axis_length));

    colors.push_back(Eigen::Vector3f(0.8f, 0.0f, 0.0f));
    colors.push_back(Eigen::Vector3f(0.8f, 0.0f, 0.0f));
    colors.push_back(Eigen::Vector3f(0.0f, 0.8f, 0.0f));
    colors.push_back(Eigen::Vector3f(0.0f, 0.8f, 0.0f));
    colors.push_back(Eigen::Vector3f(0.0f, 0.0f, 0.8f));
    colors.push_back(Eigen::Vector3f(0.0f, 0.0f, 0.8f));

    return std::make_tuple(std::move(vertices), std::move(colors));
}

std::vector<Eigen::Vector3f> primitives::GaussianRandomVertices(int sample_count,
                                                                Eigen::Vector3f mean,
                                                                float stddev)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> distrib(0.0f, stddev);

    std::vector<Eigen::Vector3f> vertices;
    vertices.reserve(sample_count);
    for (int i = 0; i < sample_count; i++)
        vertices.push_back(Eigen::Vector3f(
            distrib(gen) + mean.x(), distrib(gen) + mean.y(), distrib(gen) + mean.z()));

    return vertices;
}

std::vector<Eigen::Vector3f> primitives::GaussianRandomVertices(int sample_count,
                                                                float stddev)
{
    return primitives::GaussianRandomVertices(sample_count, Eigen::Vector3f::Zero(),
                                              stddev);
}

std::tuple<std::vector<Eigen::Vector3f>, std::vector<Triangle>> primitives::Icosahedron(
    float edge_length)
{
    if (edge_length < 0)
        return std::make_tuple(icosahedron::vertices, icosahedron::triangles);

    std::vector<Eigen::Vector3f> vertices = icosahedron::vertices;

    const float multiplier = edge_length / icosahedron::l;
    for (auto &&vertex : vertices) vertex *= multiplier;

    return std::make_tuple(std::move(vertices), icosahedron::triangles);
}

std::tuple<std::vector<Eigen::Vector3f>, std::vector<Triangle>> primitives::Icosphere(
    float radius, int detail_level)
{
    if (radius < 0)
        throw std::invalid_argument("primitives::icosphere - given radius is negative.");

    if (detail_level == 0) return primitives::Icosahedron(icosahedron::l * radius);

    if (5 < detail_level) {
        LOG_INFO(
            "primitives::icosphere max detail_level is 5, which generates enough detail "
            "- 10242 "
            "vert & 20480 faces. ( requested : {} )",
            detail_level);
        detail_level = 5;
    }

    // prep
    std::vector<Eigen::Vector3f> vertices = icosahedron::vertices;
    vertices.reserve(std::pow(4, detail_level) * 10 + 2);
    std::vector<Triangle> triangles = icosahedron::triangles;

    // subdivide
    for (int i = 0; i < detail_level; i++) {
        // First : edge <pair of two vertex id>
        // Second : new vertex id on edge
        using LUT = std::map<std::pair<unsigned int, unsigned int>, unsigned int>;
        std::vector<Triangle> new_triangles;
        LUT lut;

        for (auto &&triangle : triangles)  // each triangle
        {
            std::vector<unsigned int> midle_vertex_id(3);
            for (int j = 0; j < 3; j++)  // each edge
            {
                LUT::key_type edge(triangle.vertex_index[j],
                                   triangle.vertex_index[(j + 1) % 3]);
                if (edge.first > edge.second) std::swap(edge.first, edge.second);

                auto inserted = lut.insert({edge, vertices.size()});
                const bool &is_new_vertex = inserted.second;
                if (is_new_vertex) {
                    auto &vert1 = vertices[triangle.vertex_index[j]];
                    auto &vert2 = vertices[triangle.vertex_index[(j + 1) % 3]];
                    Eigen::Vector3f new_vertex = (vert1 + vert2).normalized();
                    vertices.push_back(new_vertex);
                }

                const auto &lut_iter = inserted.first;
                const auto &vertex_id = lut_iter->second;
                midle_vertex_id[j] = vertex_id;
            }

            new_triangles.push_back(Triangle{triangle.vertex_index[0], midle_vertex_id[0],
                                             midle_vertex_id[2]});
            new_triangles.push_back(Triangle{triangle.vertex_index[1], midle_vertex_id[1],
                                             midle_vertex_id[0]});
            new_triangles.push_back(Triangle{triangle.vertex_index[2], midle_vertex_id[2],
                                             midle_vertex_id[1]});
            new_triangles.push_back(
                Triangle{midle_vertex_id[0], midle_vertex_id[1], midle_vertex_id[2]});
        }

        triangles = std::move(new_triangles);
    }

    // give radius
    for (auto &&vertex : vertices) vertex *= radius;

    return std::make_tuple(std::move(vertices), std::move(triangles));
}

std::vector<Eigen::Vector3f> primitives::Wireframe(
    const std::tuple<std::vector<Eigen::Vector3f>, std::vector<Triangle>> &mesh)
{
    std::vector<Eigen::Vector3f> ret;
    const auto &vertices = std::get<0>(mesh);
    const auto &triangles = std::get<1>(mesh);
    ret.reserve(6 * vertices.size());  // x3(approx edge per vert) x2(vert per edge)

    using LUT = std::map<std::pair<unsigned int, unsigned int>, bool>;
    LUT lut;

    for (auto &&triangle : triangles) {
        for (int j = 0; j < 3; j++)  // each edge
        {
            LUT::key_type edge(triangle.vertex_index[j],
                               triangle.vertex_index[(j + 1) % 3]);
            if (edge.first > edge.second) std::swap(edge.first, edge.second);

            auto inserted = lut.insert({edge, true});
            const bool &is_new_edge = inserted.second;

            ret.push_back(vertices[edge.first]);
            ret.push_back(vertices[edge.second]);
        }
    }

    return ret;
}

std::vector<Eigen::Vector3f> primitives::CameraFrame(float aspect, float scale)
{
    return cameraframe::generate(-aspect, aspect, -1.0f, 1.0f, scale);
}

std::vector<Eigen::Vector3f> primitives::CameraFrame(float width, float height,
                                                     float focal_length, float scale)
{
    const float inverse_2f = 0.5f / (focal_length);
    return cameraframe::generate(-width * inverse_2f, width * inverse_2f,
                                 -height * inverse_2f, height * inverse_2f, scale);
}

std::vector<Eigen::Vector3f> primitives::CameraFrame(float width, float height, float fx,
                                                     float fy, float cx, float cy,
                                                     float scale)
{
    return cameraframe::generate(-cx / fx, (width - cx) / fx, -cy / fy,
                                 (height - cy) / fy, scale);
}

}  // namespace object
}  // namespace seg