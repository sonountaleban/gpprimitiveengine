#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Engine3D/Vector3D.h"
#include "Engine3D/Matrix.h"
#include "Engine3D/Engine3D.h"
#include "Application.h"

class Camera
{
    friend class Engine3D;
    friend class Application;

public:
    enum MovementTypes
    {
        MOVEMENT_TYPE_FORWARD,
        MOVEMENT_TYPE_BACK,
        MOVEMENT_TYPE_LEFT,
        MOVEMENT_TYPE_RIGHT
    };

    Vector3D    m_pos;
    Vector3D    m_angle;

    // ------------------------------------------------------------------

    Camera(const float posX, const float posY, const float posZ, const float angleX, const float angleY, const float angleZ);
    ~Camera();

    bool        SetScreen(const int fov, const int screenWidth, const int screenHeight, const float nearZ, const float farZ);

    void        Move(const MovementTypes movementType, const float value);
    void        Rotate(const float diffAngleX, const float diffAngleY, const float diffAngleZ);
    void        UpdateAngulation();

private:
    int         m_screenWidth;
    int         m_screenHeight;
    int         m_screenCenterX;
    int         m_screenCenterY;
    int         *m_pScreenBuffer;
    int         *m_pZBuffer;
    float       m_screenDistX;
    float       m_screenDistY;
    float       m_nearZ;
    float       m_farZ;
    Matrix      m_matrixRot;
    Matrix      m_matrixMove;

    float       m_zBufferA;
    float       m_zBufferB;
    Vector3D    m_vectorTemp1;
    Vector3D    m_vectorTemp2;

    // ------------------------------------------------------------------

    void        ClearZBuffer();
    bool        SetValueZBuffer(const int offset, const float z, const bool check);
};

#endif
