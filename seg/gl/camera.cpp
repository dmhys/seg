#include "seg/gl/camera.h"

#include <Eigen/Dense>
#include <cmath>

#include "seg/core/config.h"
#include "seg/utilities/logger.h"

static const float PI = std::acos(-1);
inline float to_rad(float degree) { return degree * PI / 180.0f; }

// rotates world orientation to camera matrix
static const Eigen::Matrix3f to_zfront =
    (Eigen::Matrix3f() << 0, 0, 1, 1, 0, 0, 0, 1, 0).finished();

namespace seg {
namespace gl {
Camera::Camera() { view_matrix.setIdentity(); }

void Camera::init(int width, int height)
{
    updateViewMatrix();
    onScreenResize(width, height);
}

void Camera::onScreenResize(int width, int height)
{
    window_size << width, height;
    inverse_aspect = (float)height / (float)width;
    updateProjectionMatrix();
}

// Mouse Event Handlers =================================================
void Camera::onMouseClick(int btn, int pos_x, int pos_y)
{
    onMouseClick(static_cast<MouseButton>(btn), Eigen::Vector2i(pos_x, pos_y));
}

void Camera::onMouseClick(int btn, Eigen::Vector2i abs_pos)
{
    onMouseClick(static_cast<MouseButton>(btn), abs_pos);
}

void Camera::onMouseClick(MouseButton btn, int pos_x, int pos_y)
{
    onMouseClick(btn, Eigen::Vector2i(pos_x, pos_y));
}

void Camera::onMouseClick(MouseButton btn, Eigen::Vector2i abs_pos)
{
    current_clicked_button = btn;
    last_mouse_pos = abs_pos;
}

void Camera::onMouseDrag(int pos_x, int pos_y) { onMouseDrag(Eigen::Vector2i(pos_x, pos_y)); }

void Camera::onMouseDrag(Eigen::Vector2i abs_pos)
{
    Eigen::Vector2f relative_delta = absToRelative(abs_pos - last_mouse_pos);
    last_mouse_pos = abs_pos;

    const float scale_factor_rotation = 1.5f;
    const float scale_factor_translation = 1.0f;
    switch (current_clicked_button) {
        case MOUSE_LEFT:                                       // rotation
            relative_delta *= 180.0f * scale_factor_rotation;  // to degree
            latitude -= relative_delta.x();
            longitude += relative_delta.y();

            // clamp
            longitude = (90.0f < longitude) ? 90.0f : longitude;
            longitude = (longitude < -90.0f) ? -90.0f : longitude;

            latitude = (360.0f < latitude) ? (latitude - 360.0f) : latitude;
            latitude = (latitude < 0.0f) ? (latitude + 360.0f) : latitude;
            break;
        case MOUSE_RIGHT:  // move
        {
            relative_delta *= distance * scale_factor_translation;
            Eigen::Matrix3f orientation =
                (Eigen::AngleAxisf(to_rad(latitude), Eigen::Vector3f::UnitZ()) *
                 Eigen::AngleAxisf(-to_rad(longitude), Eigen::Vector3f::UnitY()))
                    .toRotationMatrix();
            Eigen::Vector3f translation =
                orientation * (Eigen::Vector3f(0.0f, -relative_delta.x(), relative_delta.y()));
            center += translation;
            break;
        }
        case MOUSE_WHEEL:  // nothing
            break;
        default:
            break;
    }
    updateViewMatrix();
}

Eigen::Vector2f Camera::absToRelative(const Eigen::Vector2i& delta) const
{
    return Eigen::Vector2f(static_cast<float>(delta.x()) / static_cast<float>(window_size.x()),
                           static_cast<float>(delta.y()) /
                               static_cast<float>(window_size.x()));  // intentional divisor x()
}

void Camera::onMouseWheel(int scroll)
{
    if (scroll > 0)  // zoom in
    {
        if (distance < 0.2f) return;
        distance *= 0.9f;
    } else  // zoom out.
    {
        if ((far * 0.25) < distance) return;
        distance *= 1.1f;
    }

    getConfig().camera_distance = distance;

    updateViewMatrix();
}

// View matrix functions ================================================
void Camera::updateViewMatrix()
{
    // orientation -> pose of current camera with lat/long rotation only
    Eigen::Matrix3f orientation = (Eigen::AngleAxisf(to_rad(latitude), Eigen::Vector3f::UnitZ()) *
                                   Eigen::AngleAxisf(-to_rad(longitude), Eigen::Vector3f::UnitY()))
                                      .toRotationMatrix();
    view_matrix.topLeftCorner<3, 3>() = (orientation * to_zfront).transpose();
    view_matrix.topRightCorner<3, 1>() = -view_matrix.topLeftCorner<3, 3>() *
                                         (orientation * Eigen::Vector3f(distance, 0, 0) + center);
}

// projection matrix fuctions ===========================================
void Camera::setProjectionMode(int mode)
{
    setProjectionMode(static_cast<Camera::ProjectionMode>(mode));
}

void Camera::setProjectionMode(Camera::ProjectionMode _mode)
{
    mode = _mode;
    updateProjectionMatrix();
}

void Camera::setNear(float value)
{
    near = near;
    updateProjectionMatrix();
}

void Camera::setFar(float value)
{
    far = value;
    updateProjectionMatrix();
}

void Camera::setHorizontalFov(float value)
{
    hfov = value;
    if (mode == PERSPECTIVE)  // no need to update when ortho
        updateProjectionMatrixPerspective();
}

void Camera::updateProjectionMatrix()
{
    if (mode == ORTHOGRAPHIC) updateProjectionMatrixOrtho();

    if (mode == PERSPECTIVE) updateProjectionMatrixPerspective();
}

void Camera::updateProjectionMatrixOrtho()
{
    const float horizontal_box_size = 3.0f;
    projection_matrix << (2.0f / horizontal_box_size), 0, 0, 0, 0,
        (1.0f / (horizontal_box_size * inverse_aspect)), 0, 0, 0, 0, -2.0f / (far - near),
        -(far + near) / (far - near), 0, 0, 0, 1;
}

void Camera::updateProjectionMatrixPerspective()
{
    const float half_width_over_depth = std::tan(hfov / 360.0f * PI);
    projection_matrix << (1.0f / half_width_over_depth), 0, 0, 0, 0,
        (1.0f / (inverse_aspect * half_width_over_depth)), 0, 0, 0, 0, -(far + near) / (far - near),
        -(2 * far * near) / (far - near), 0, 0, -1, 0;
}

}  // namespace gl
}  // namespace seg