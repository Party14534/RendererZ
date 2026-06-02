#ifndef CAMERA_H
#define CAMERA_H

#include "Math/math.h"
#include "Math/vec.h"
#include "Math/mat.h"

struct Camera {
    public:
    Camera();
    Camera(Vec3 _pos, Vec3 _direction);

    void LookAt(const Vec3 point);

    // Getters and Setters
    Vec3 GetPos();
    void SetPos(const Vec3 _pos);
    void MovePos(const Vec3 _pos);
    Vec3 GetDirection();
    void SetDirection(const Vec3 _direction);
    void MoveDirection(const Vec2 _change);
    Vec3 GetCamFront();
    Vec3 GetCamUp();
    Vec3 GetCamRight();
    Mat GetViewMatrix();
    Mat GetProjectionMatrix();

    void BuildPerspectiveMatrices(u32 width, u32 height);

    private:
    Vec3 pos;
    Vec3 direction;
    Vec3 camFront;
    Vec3 camUp;
    Vec3 camRight;
    Mat viewMatrix;

    Mat orthoMat;
    Mat perspMat;
    Mat viewMat;
    float aspectRatio;
    float fov;
    float yaw, pitch;
    bool isOrthographic = false;

    void calculateViewMatrix();
};

#endif
