#ifndef SEG_OBJECT_PRIMITIVES_H
#define SEG_OBJECT_PRIMITIVES_H

#include <Eigen/Dense>
#include <tuple>
#include <vector>

#include "seg/types.h"

namespace seg {
namespace object {
// note - primitives are struct, to make static methods available.
// Thus, every functions are Pascal-cased like other "classes"
struct primitives
{
    /**
     * @brief Returns grid with given Grid / Cell size. Cell starts tiling from (0,0)
     * @param max maximum grid boundary ( positive value )
     * @param min minimum grid boundary ( negative value )
     * @return vertices of Grid to use with GL_LINE
     */
    static std::vector<Eigen::Vector3f> Grid(Eigen::Vector2f max, Eigen::Vector2f min,
                                             Eigen::Vector2f cell_size);

    /**
     * @brief Returns grid with given Grid / Cell size. Cell starts tiling from (0,0)
     * @param grid_size grid size, from (max,max) to (-max,-max)
     * @param cell_size cell size, each cell is square
     * @return vertices of Grid to use with GL_LINE
     */
    static inline std::vector<Eigen::Vector3f> Grid(float grid_size, float cell_size)
    {
        return Grid(Eigen::Vector2f(grid_size, grid_size), Eigen::Vector2f(-grid_size, -grid_size),
                    Eigen::Vector2f(cell_size, cell_size));
    }

    /**
     * @brief Returns X/Y/Z axis and color as tuple.
     * @param axis_length - length of each axis
     * @return tuple - <1> std::vector<Eigen::Vector3f> vertices x/y/z
     *         tuple - <2> std::vector<Eigen::Vector3f> colors r/g/b
     */
    static std::tuple<std::vector<Eigen::Vector3f>, std::vector<Eigen::Vector3f>> Axis(
        float axis_length = 1.0f);

    /**
     * @brief Returns Gaussian Distributed Random Vector3f samples.
     * @param sample_count - number of samples
     * @param mean mean of samples
     * @param stddev standard deviations of samples
     */
    static std::vector<Eigen::Vector3f> GaussianRandomVertices(
        int sample_count, Eigen::Vector3f mean = Eigen::Vector3f::Zero(), float stddev = 1.0f);

    static std::vector<Eigen::Vector3f> GaussianRandomVertices(int sample_count, float stddev);

    /**
     * @brief Generates icosahedron(polyhedron with 20 faces)
     * @param edge_length - length of each edge.
     *        if not given, icosahedron inscribed to unit sphere will be returned.
     *        ( Default length of each edge = 1.051 )
     * @return tuple - <1> std::vector<Eigen::Vector3f> vertices x/y/z (12)
     *         tuple - <2> std::vector<Triangle> vertex index of each faces(20)
     */
    static std::tuple<std::vector<Eigen::Vector3f>, std::vector<Triangle>> Icosahedron(
        float edge_length = -1.0f);

    /**
     * @brief Generates sphere, using subdivision iteration starting from icosahedron.
     * @param radius - radius of sphere
     * @param detail_level - Iteration number of subdivision. The more, the smooth sphere will be.
     *                       detail_level 0 means no iteration, resulting icosahedron.
     * @return tuple - <1> std::vector<Eigen::Vector3f> vertices x/y/z
     *         tuple - <2> std::vector<Triangle> vertex index of each faces
     */
    static std::tuple<std::vector<Eigen::Vector3f>, std::vector<Triangle>> Icosphere(
        float radius, int detail_level = 3);

    /**
     * @brief From given mesh, extracts wireframe(edges).
     * @return Line Segments.(0-1 2-3 4-5, which means duplicated vertices)
     */
    static std::vector<Eigen::Vector3f> Wireframe(
        const std::tuple<std::vector<Eigen::Vector3f>, std::vector<Triangle>>& mesh);

    /**
     * @brief Returns camera frame Line Segments to render.
     *        Default vertical field of view - 90 degrees.
     * @param aspect - width/height
     * @param scale - projection center to image.
     * @return Line Segments. (8 lines)
     */
    static std::vector<Eigen::Vector3f> CameraFrame(float aspect = 1.5, float scale = 1.0f);

    /**
     * @brief Returns camera frame Line Segments to render.
     *        note - horizontal fov = 2*atan(width/focal_length)
     * @return Line Segments. (8 lines)
     */
    static std::vector<Eigen::Vector3f> CameraFrame(float width, float height, float focal_length, float scale = 1.0f);

    /**
     * @brief Returns camera frame Line Segments to render, from camera intrinsics.
     * @return Line Segments. (8 lines)
     */
    static std::vector<Eigen::Vector3f> CameraFrame(float width, float height, float fx, float fy, float cx, float cy, float scale = 1.0f);
};  // struct primitives
}  // namespace object
}  // namespace seg

#endif