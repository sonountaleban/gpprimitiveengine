#include "Engine3D/Vector2D.h"

Vector2D::Vector2D()
{
    Set(0.0f, 0.0f);
}

Vector2D::Vector2D(const float x, const float y)
{
    Set(x, y);
}

Vector2D::~Vector2D()
{
}

void Vector2D::Set(const float x, const float y)
{
    m_x = x;
    m_y = y;
}

void Vector2D::CopyVector(Vector2D *pDst, const Vector2D *pSrc)
{
    if (pSrc && pDst)
    {
        pDst->m_x = pSrc->m_x;
        pDst->m_y = pSrc->m_y;
    }
}

void Vector2D::AddVectors(Vector2D *pDst, const Vector2D *pSrc1, const Vector2D *pSrc2)
{
    if (pSrc1 && pSrc2 && pDst)
    {
        pDst->m_x = pSrc1->m_x + pSrc2->m_x;
        pDst->m_y = pSrc1->m_y + pSrc2->m_y;
    }
}

void Vector2D::SubVectors(Vector2D *pDst, const Vector2D *pSrc1, const Vector2D *pSrc2)
{
    if (pSrc1 && pSrc2 && pDst)
    {
        pDst->m_x = pSrc1->m_x - pSrc2->m_x;
        pDst->m_y = pSrc1->m_y - pSrc2->m_y;
    }
}

void Vector2D::InterpVectors(Vector2D *pDst, const Vector2D *pSrc1, const Vector2D *pSrc2, const float rate)
{
    if (pSrc1 && pSrc2 && pDst)
    {
        pDst->m_x = pSrc1->m_x + (pSrc2->m_x - pSrc1->m_x) * rate;
        pDst->m_y = pSrc1->m_y + (pSrc2->m_y - pSrc1->m_y) * rate;
    }
}

void Vector2D::SwapVectors(Vector2D *pVector1, Vector2D *pVector2)
{
    float x;
    float y;

    if (!pVector1 || !pVector2)
    {
        return;
    }

    x = pVector1->m_x;
    y = pVector1->m_y;

    pVector1->m_x = pVector2->m_x;
    pVector1->m_y = pVector2->m_y;

    pVector2->m_x = x;
    pVector2->m_y = y;
}

bool Vector2D::IntersectVectors(const Vector2D *pVectorA, const Vector2D *pVectorB, const Vector2D *pVectorC, const Vector2D *pVectorD)
{
    float r;
    float s;
    float den;

    if (!pVectorA || !pVectorB || !pVectorC || !pVectorD)
    {
        return false;
    }

    den = (pVectorB->m_x - pVectorA->m_x) * (pVectorD->m_y - pVectorC->m_y) - (pVectorB->m_y - pVectorA->m_y) * (pVectorD->m_x - pVectorC->m_x);
    if (den == 0.0f)
    {
        return false;
    }

    r = ((pVectorA->m_y - pVectorC->m_y) * (pVectorD->m_x - pVectorC->m_x) - (pVectorA->m_x - pVectorC->m_x) * (pVectorD->m_y - pVectorC->m_y)) / den;
    s = ((pVectorA->m_y - pVectorC->m_y) * (pVectorB->m_x - pVectorA->m_x) - (pVectorA->m_x - pVectorC->m_x) * (pVectorB->m_y - pVectorA->m_y)) / den;
    if (r >= 0.0f && r <= 1.0f && s >= 0.0f && s <= 1.0f)
    {
        return true;
    }
    else
    {
        return false;
    }
}

float Vector2D::SignVectors(const Vector2D *pVectorA, const Vector2D *pVectorB, const Vector2D *pVectorC)
{
    if (!pVectorA || !pVectorB || !pVectorC)
    {
        return 0.0f;
    }
    else
    {
        return (pVectorA->m_x - pVectorC->m_x) * (pVectorB->m_y - pVectorC->m_y) - (pVectorB->m_x - pVectorC->m_x) * (pVectorA->m_y - pVectorC->m_y);
    }
}

bool Vector2D::PointInTriangle(const Vector2D *pPoint, const Vector2D *pVectorA, const Vector2D *pVectorB, const Vector2D *pVectorC)
{
    bool b1;
    bool b2;
    bool b3;

    if (!pPoint || !pVectorA || !pVectorB || !pVectorC)
    {
        return false;
    }

    b1 = SignVectors(pPoint, pVectorA, pVectorB) < 0.0f;
    b2 = SignVectors(pPoint, pVectorB, pVectorC) < 0.0f;
    b3 = SignVectors(pPoint, pVectorC, pVectorA) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}
