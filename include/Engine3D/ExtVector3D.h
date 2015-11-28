#ifndef _EXT_VECTOR3D_H_
#define _EXT_VECTOR3D_H_

#include "Engine3D/Color.h"
#include "Engine3D/Vector2D.h"
#include "Engine3D/Vector3D.h"
#include "Engine3D/Engine3D.h"

class Engine3D;

class ExtVector3D
{
    friend class Engine3D;

public:
    bool        m_processColor;
    bool        m_trasformTo2D;

    int         m_xOnScreen;
    int         m_yOnScreen;

    Color       m_color;
    Vector2D    m_uv;
    Vector3D    m_normal;
    Vector3D    m_pos;

    // ------------------------------------------------------------------

    ExtVector3D();
    ExtVector3D(const float x, const float y, const float z);
    ExtVector3D(const float x, const float y, const float z, const Color *pColor);
    ExtVector3D(const float x, const float y, const float z, const Color *pColor, const float u, const float v);
    ~ExtVector3D();

    void        Reset();

    void        TrasformTo2D(const int screenCenterX, const int screenCenterY, const float screenDistX, const float screenDistY);
    bool        IsVisible();

private:
    Color       m_colorProcessed;

    bool        m_onScreen;

    // ------------------------------------------------------------------
};

#endif
