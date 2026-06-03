#include "Camera.h"

Camera::Camera() :
    viewMatrix(4, 4),
    orthoMat(4, 4),
    perspMat(4, 4),
    viewMat(Mat::getIdentity(4))
{
    pos = Vec3(0, 0, 3);
    yaw = 90.f;
    pitch = 0.f;
    direction.x = cos(radians(yaw)) * cos(radians(pitch));
    direction.y = sin(radians(pitch));
    direction.z = sin(radians(yaw)) * cos(radians(pitch));
    direction = direction.normalize();

    camFront = direction * -1.f; // actual look direction (forward)
    camRight = Vec3(0, 1, 0).cross(direction).normalize();
    camUp = direction.cross(camRight);

    calculateViewMatrix();
}

void Camera::LookAt(const Vec3 point) {
    direction = (pos - point).normalize();

    camRight = Vec3(0, 1, 0).cross(direction).normalize();
    camUp = direction.cross(camRight);

    calculateViewMatrix();
}

Vec3 Camera::GetPos() { return pos; }
void Camera::SetPos(const Vec3 _pos) { 
    pos = _pos; 
    calculateViewMatrix();
}
void Camera::MovePos(const Vec3 _pos) { 
    pos = pos + _pos; 
    calculateViewMatrix();
}

Vec3 Camera::GetDirection() { return direction; }
void Camera::SetDirection(const Vec3 _direction) { 
    direction = _direction; 
    calculateViewMatrix();
}
void Camera::MoveDirection(const Vec2 change) {
    yaw += change.x;
    pitch += change.y;
    pitch = std::clamp(pitch, -89.0f, 89.0f);

    // direction is the view-matrix z-axis: it points from the target back
    // toward the camera (opposite the look direction), matching LookAt().
    direction.x = cos(radians(yaw)) * cos(radians(pitch));
    direction.y = sin(radians(pitch));
    direction.z = sin(radians(yaw)) * cos(radians(pitch));
    direction = direction.normalize();

    // Rebuild the full orthonormal basis from the new direction so the view
    // matrix stays a proper rotation (otherwise the scene shears/stretches).
    camFront = direction * -1.f; // actual look direction (forward)
    camRight = Vec3(0, 1, 0).cross(direction).normalize();
    camUp = direction.cross(camRight);

    calculateViewMatrix();
}

Vec3 Camera::GetCamFront() { return camFront; }
Vec3 Camera::GetCamUp() { return camUp; }
Vec3 Camera::GetCamRight() { return camRight; }

Mat Camera::GetViewMatrix() { return viewMatrix; }
Mat Camera::GetProjectionMatrix() { return isOrthographic ? orthoMat : perspMat; }

/*
 * Private functions
 */
// Look at matrix
void Camera::calculateViewMatrix() {
    Mat m1 = Mat(4, 4, {
        camRight.x, camRight.y, camRight.z, 0,
        camUp.x, camUp.y, camUp.z, 0,
        direction.x, direction.y, direction.z, 0,
        0, 0, 0, 1
    });

    Mat m2 = Mat(4, 4, {
        1, 0, 0, -pos.x,
        0, 1, 0, -pos.y,
        0, 0, 1, -pos.z,
        0, 0, 0, 1
    });

    viewMatrix = m1 * m2;
}

void Camera::BuildPerspectiveMatrices(u32 width, u32 height) {
    float near = 0.1;
    float far = 50000;
    float left = 0;
    float right = width;
    float bottom = 0;
    float top = height;

    fov = 90;

    aspectRatio = (float)width / (float)height;
    orthoMat = Mat(4, 4, {
            2/(right - left), 0, 0, -((right + left) / (right - left)),
            0, 2/(top - bottom), 0, -((top + bottom) / (top - bottom)),
            0, 0, -2/(far - near), -((far + near) / (far - near)),
            0, 0, 0, 1
        }
    );

    float S = 1.f / (tan((fov * 0.5f) * (PI * (1.f / 180.f))));
    perspMat = Mat(4, 4, {
            S / aspectRatio, 0, 0, 0,
            0, S, 0, 0,
            0, 0, -((far + near) / (far - near)), -((2.f * far * near) / (far - near)),
            0, 0, -1, 0
        }
    );
}
