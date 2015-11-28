#include "utils.h"
#include "Engine3D/Camera.h"

Camera::Camera(const float posX, const float posY, const float posZ, const float angleX, const float angleY, const float angleZ)
{
    m_pos.Set(posX, posY, posZ);
    m_angle.Set(angleX, angleY, angleZ);

    m_screenWidth = 0;
    m_screenHeight = 0;
    m_screenCenterX = 0;
    m_screenCenterY = 0;
    m_pScreenBuffer = NULL;
    m_pZBuffer = NULL;
    m_screenDistX = 0.0f;
    m_screenDistY = 0.0f;
    m_nearZ = 0.0f;
    m_farZ = 0.0f;

    m_zBufferA = 0.0f;
    m_zBufferB = 0.0f;

    UpdateAngulation();
}

Camera::~Camera()
{
    if (m_pScreenBuffer)
    {
        delete[] m_pScreenBuffer;
    }

    if (m_pZBuffer)
    {
        delete[] m_pZBuffer;
    }
}

bool Camera::SetScreen(const int fov, const int screenWidth, const int screenHeight, const float nearZ, const float farZ)
{
    float angle = (float)M_PI / 180.0f * (float)fov;

    if (fov <= 0 || fov >= 180 || screenWidth <= 2 || screenHeight <= 2 || fabs(nearZ) <= TOLLERANCE || farZ - nearZ <= TOLLERANCE)
    {
        return false;
    }

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_screenCenterX = screenWidth >> 1;
    m_screenCenterY = screenHeight >> 1;
    m_screenDistX = (float)screenWidth / 2.0f * tanf(angle);
    m_screenDistY = (float)screenHeight / 2.0f * tanf(angle);
    m_nearZ = nearZ;
    m_farZ = farZ;

    m_zBufferA = m_farZ / (m_farZ - m_nearZ);
    m_zBufferB = m_farZ * m_nearZ / (m_nearZ - m_farZ);

    if (m_pScreenBuffer)
    {
        delete[] m_pScreenBuffer;
        m_pScreenBuffer = NULL;
    }

    if (m_pZBuffer)
    {
        delete[] m_pZBuffer;
        m_pZBuffer = NULL;
    }

    m_pScreenBuffer = new int[screenWidth * screenHeight];
    if (!m_pScreenBuffer)
    {
        return false;
    }

    m_pZBuffer = new int[screenWidth * screenHeight];
    if (!m_pZBuffer)
    {
        delete[] m_pScreenBuffer;
        m_pScreenBuffer = NULL;

        return false;
    }

    return true;
}

void Camera::Move(const MovementTypes movementType, const float value)
{
    m_vectorTemp1.m_x = 0.0f;
    m_vectorTemp1.m_y = 0.0f;
    m_vectorTemp1.m_z = 0.0f;

    switch (movementType)
    {
        case MOVEMENT_TYPE_FORWARD:
            m_vectorTemp1.m_z = value;

            break;

        case MOVEMENT_TYPE_BACK:
            m_vectorTemp1.m_z = -value;

            break;

        case MOVEMENT_TYPE_LEFT:
            m_vectorTemp1.m_x = -value;

            break;

        case MOVEMENT_TYPE_RIGHT:
            m_vectorTemp1.m_x = value;

            break;

        default:
            return;
    }

    Vector3D::MultiplyVectorMatrix(&m_vectorTemp2, &m_vectorTemp1, &m_matrixMove);
    Vector3D::AddVectors(&m_pos, &m_pos, &m_vectorTemp2);
}

void Camera::Rotate(const float diffAngleX, const float diffAngleY, const float diffAngleZ)
{
    float angleX = m_angle.m_x + diffAngleX;
    float angleY = m_angle.m_y + diffAngleY;
    float angleZ = m_angle.m_z + diffAngleZ;

    m_angle.Set(angleX, angleY, angleZ);

    UpdateAngulation();
}

void Camera::UpdateAngulation()
{
    m_matrixRot.SetIdentity();
    m_matrixRot.Rotate(m_angle.m_x, m_angle.m_y, m_angle.m_z);

    Matrix::InvertMatrix(&m_matrixMove, &m_matrixRot);
}

void Camera::ClearZBuffer()
{
    int i;
    int size = m_screenWidth * m_screenHeight;
    int value;

    if (!m_pZBuffer)
    {
        return;
    }

    SetValueZBuffer(0, m_farZ, false);

    value = m_pZBuffer[0];

    for (i = 1; i < size; i ++)
    {
        m_pZBuffer[i] = value;
    }
}

bool Camera::SetValueZBuffer(const int offset, const float z, const bool check)
{
    int value;

    if (!m_pZBuffer || z == 0.0f || offset < 0 || offset >= m_screenWidth * m_screenHeight)
    {
        return false;
    }

    value = (int)((float)(1 << 31) * (m_zBufferA + m_zBufferB / z));

    if (check && value < m_pZBuffer[offset])
    {
        return false;
    }

    m_pZBuffer[offset] = value;

    return true;
}
