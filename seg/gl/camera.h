#ifndef SEG_GL_CAMERA_H
#define SEG_GL_CAMERA_H

#include <Eigen/Dense>

#include "seg/types.h"

namespace seg {
namespace gl {
class Scene;

class Camera
{
   public:
    enum ProjectionMode
    {
        ORTHOGRAPHIC,
        PERSPECTIVE,
    };

    enum MouseButton
    {
        MOUSE_LEFT = 0,
        MOUSE_RIGHT = 1,
        MOUSE_WHEEL = 2,
        MOUSE_NONE = -1,
    };

   public:
    Camera();
    void init(int width, int height);
    void onScreenResize(int width, int height);

    // Mouse Events
    void onMouseClick(int btn, int pos_x, int pos_y);
    void onMouseClick(int btn, Eigen::Vector2i abs_pos);
    void onMouseClick(MouseButton btn, int pos_x, int pos_y);
    void onMouseClick(MouseButton btn, Eigen::Vector2i abs_pos);

    void onMouseDrag(int pos_x, int pos_y);
    void onMouseDrag(Eigen::Vector2i abs_pos);
    Eigen::Vector2f absToRelative(const Eigen::Vector2i& delta) const;

    void onMouseWheel(int scroll);

    // Projection
    void setProjectionMode(int mode);
    void setProjectionMode(ProjectionMode mode);
    void setNear(float value);
    void setFar(float value);
    void setHorizontalFov(float value);

    // getters
    Eigen::Vector3f getCameraLookAt() const { return center; }
    const Eigen::Matrix4f& getViewMatrix() const { return view_matrix; }
    const Eigen::Matrix4f& getProjectionMatrix() const { return projection_matrix; }

   private:
    void updateViewMatrix();

    void updateProjectionMatrix();
    void updateProjectionMatrixOrtho();
    void updateProjectionMatrixPerspective();

    // MouseEvents
    Eigen::Vector2i window_size;
    Eigen::Vector2i last_mouse_pos;
    MouseButton current_clicked_button = MOUSE_NONE;

    // View
    Eigen::Vector3f center = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
    float distance = 10.0f;
    float latitude = -135.0f;
    float longitude = 45.0f;

    // Projection
    ProjectionMode mode = PERSPECTIVE;
    float hfov = 50.0f;
    float inverse_aspect;  // height/width
    float near = 0.1f;
    float far = 5000.0f;

    // matrix
    Eigen::Matrix4f view_matrix;
    Eigen::Matrix4f projection_matrix;

};  // class Camera
}  // namespace gl
}  // namespace seg

#endif