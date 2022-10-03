#ifndef SEG_TYPES_H
#define SEG_TYPES_H

#include <Eigen/Dense>

namespace seg {
struct WindowSize
{
    int width;
    int height;

    WindowSize(int _width = 0, int _height = 0) : width(_width), height(_height) {}
};

struct RGBA
{
    float r;
    float g;
    float b;
    float a;

    RGBA(float _r = 0.0f, float _g = 0.0f, float _b = 0.0f, float _a = 0.0f)
        : r(_r), g(_g), b(_b), a(_a)
    {
    }
    inline Eigen::Vector4f& asEigenVector4f()
    {
        return *(reinterpret_cast<Eigen::Vector4f*>(this));
    }
};

struct Position
{
    int x;
    int y;

    Position(int _x = 0, int _y = 0) : x(_x), y(_y) {}
    inline Eigen::Vector2i& asEigenVector2i()
    {
        return *(reinterpret_cast<Eigen::Vector2i*>(this));
    }
};

// 3D ==============================================
struct Edge
{
    unsigned int vertex_index[2];
};

struct Triangle
{
    unsigned int vertex_index[3];
};

// 2D ==============================================
typedef WindowSize ImageSize;

enum class ImageType
{
    FLOAT_1,
    FLOAT_3,
    UCHAR_1,
    UCHAR_8,
};

}  // namespace seg

#endif