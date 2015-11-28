#include "Engine3D/Light.h"

Light::Light()
{
    m_type = TYPE_AMBIENT;
}

Light::Light(const Color *pColor)
{
    m_type = TYPE_AMBIENT;

    if (pColor)
    {
        Color::CopyColor(&m_color, pColor);
    }
}

Light::Light(const Color *pColor, const float angleX, const float angleY)
{
    m_type = TYPE_DIRECTIONAL;

    m_angle.Set(angleX, angleY, 0.0f);

    if (pColor)
    {
        Color::CopyColor(&m_color, pColor);
    }

    UpdateAngulation();
}

Light::Light(const Color *pColor, const float posX, const float posY, const float posZ)
{
    m_type = TYPE_POINT;

    m_pos.Set(posX, posY, posZ);

    if (pColor)
    {
        Color::CopyColor(&m_color, pColor);
    }
}

Light::Light(const Color *pColor, const float angleX, const float angleY, const float posX, const float posY, const float posZ)
{
    m_type = TYPE_SPOT;

    m_pos.Set(posX, posY, posZ);

    m_angle.Set(angleX, angleY, 0.0f);

    if (pColor)
    {
        Color::CopyColor(&m_color, pColor);
    }

    UpdateAngulation();
}

Light::~Light()
{
}

void Light::UpdateAngulation()
{
    Vector3D vectorTemp(0.0f, 0.0f, 1.0f);

    m_matrixRot.SetIdentity();
    m_matrixRot.Rotate(m_angle.m_x, m_angle.m_y, m_angle.m_z);

    Vector3D::MultiplyVectorMatrix(&m_vectorRot, &vectorTemp, &m_matrixRot);
}
