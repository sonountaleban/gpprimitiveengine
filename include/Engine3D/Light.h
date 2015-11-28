#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Engine3D/Color.h"
#include "Engine3D/Vector3D.h"
#include "Engine3D/Matrix.h"
#include "Engine3D/Engine3D.h"

class Light
{
    friend class Engine3D;

public:
    enum Types
    {
        TYPE_NONE = -1,
        TYPE_AMBIENT = 0,
        TYPE_DIRECTIONAL,
        TYPE_POINT,
        TYPE_SPOT
    };

    Types       m_type;
    Vector3D    m_pos;
    Vector3D    m_angle;
    Color       m_color;
    Vector3D    m_vectorRot;

    // ------------------------------------------------------------------

    Light();
    Light(const Color *pColor);
    Light(const Color *pColor, const float angleX, const float angleY);
    Light(const Color *pColor, const float posX, const float posY, const float posZ);
    Light(const Color *pColor, const float angleX, const float angleY, const float posX, const float posY, const float posZ);
    ~Light();

    void        UpdateAngulation();

private:
    Matrix      m_matrixRot;
};

#endif
