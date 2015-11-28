#include "utils.h"
#include "Engine3D/ExtVector3D.h"

ExtVector3D::ExtVector3D()
{
    m_processColor = true;

    Reset();
}

ExtVector3D::ExtVector3D(const float x, const float y, const float z)
{
    m_processColor = true;

    Reset();

    m_pos.Set(x, y, z);
}

ExtVector3D::ExtVector3D(const float x, const float y, const float z, const Color *pColor)
{
    m_processColor = true;

    Reset();

    m_pos.Set(x, y, z);

    if (pColor)
    {
        Color::CopyColor(&m_color, pColor);
    }
}

ExtVector3D::ExtVector3D(const float x, const float y, const float z, const Color *pColor, const float u, const float v)
{
    m_processColor = true;

    Reset();

    m_pos.Set(x, y, z);

    if (pColor)
    {
        Color::CopyColor(&m_color, pColor);
    }

    m_uv.Set(u, v);
}

ExtVector3D::~ExtVector3D()
{
}

void ExtVector3D::Reset()
{
    m_trasformTo2D = false;

    m_onScreen = false;
    m_xOnScreen = -1;
    m_yOnScreen = -1;

    m_normal.Set(0.0f, 0.0f, 0.0f);
}

void ExtVector3D::TrasformTo2D(const int screenCenterX, const int screenCenterY, const float screenDistX, const float screenDistY)
{
    if (m_trasformTo2D)
    {
        if (fabs(m_pos.m_z) > TOLLERANCE)
        {
            m_onScreen = true;

            m_xOnScreen = screenCenterX + (int)(screenDistX * m_pos.m_x / m_pos.m_z);
            m_yOnScreen = screenCenterY + (int)(screenDistY * m_pos.m_y / m_pos.m_z);
        }
    }
}

bool ExtVector3D::IsVisible()
{
    return m_onScreen;
}
