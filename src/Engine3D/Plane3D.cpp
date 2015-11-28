#include "Engine3D/Plane3D.h"

Plane3D::Plane3D()
{
    Set(0.0f, 0.0f, 0.0f, 0.0f);
}

Plane3D::Plane3D(const float a, const float b, const float c, const float d)
{
    Set(a, b, c, d);
}

Plane3D::~Plane3D()
{
}

void Plane3D::Set(const float a, const float b, const float c, const float d)
{
    m_a = a;
    m_b = b;
    m_c = c;
    m_d = d;

    m_normal.Set(a, b, c);
}

void Plane3D::Set(const Vector3D *pVector1, const Vector3D *pVector2, const Vector3D *pVector3)
{
    Vector3D edge1;
    Vector3D edge2;

    if (!pVector1 || !pVector2 || !pVector3)
    {
        return;
    }

    Vector3D::SubVectors(&edge1, pVector3, pVector1);
    Vector3D::SubVectors(&edge2, pVector2, pVector1);

    Vector3D::CrossProductVectors(&m_normal, &edge1, &edge2);
    Vector3D::NormalizeVector(&m_normal, &m_normal);

    m_a = m_normal.m_x;
    m_b = m_normal.m_y;
    m_c = m_normal.m_z;
    m_d = -Vector3D::DotProductVectors(&m_normal, pVector1);
}

float Plane3D::GetDistanceFromVector(const Vector3D *pVector)
{
    if (pVector)
    {
        return pVector->m_x * m_a + pVector->m_y * m_b + pVector->m_z * m_c + m_d;
    }
    else
    {
        return 0.0f;
    }
}

void Plane3D::CopyPlane(Plane3D *pDst, const Plane3D *pSrc)
{
    if (pSrc && pDst)
    {
        pDst->m_a = pSrc->m_a;
        pDst->m_b = pSrc->m_b;
        pDst->m_c = pSrc->m_c;
        pDst->m_d = pSrc->m_d;

        Vector3D::CopyVector(&pDst->m_normal, &pSrc->m_normal);
    }
}
