#ifndef _PLANE3D_H_
#define _PLANE3D_H_

#include "Engine3D/Vector3D.h"

class Plane3D
{
public:
    float       m_a;
    float       m_b;
    float       m_c;
    float       m_d;
    Vector3D    m_normal;

    // ------------------------------------------------------------------

    Plane3D();
    Plane3D(const float a, const float b, const float c, const float d);
    ~Plane3D();

    void        Set(const float a, const float b, const float c, const float d);
    void        Set(const Vector3D *pVector1, const Vector3D *pVector2, const Vector3D *pVector3);
    float       GetDistanceFromVector(const Vector3D *pVector);

    static void CopyPlane(Plane3D *pDst, const Plane3D *pSrc);
};

#endif
