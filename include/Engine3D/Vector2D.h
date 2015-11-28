#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_

class Vector2D
{
public:
    float           m_x;
    float           m_y;

    // ------------------------------------------------------------------

    Vector2D();
    Vector2D(const float x, const float y);
    ~Vector2D();

    void            Set(const float x, const float y);

    static void     CopyVector(Vector2D *pDst, const Vector2D *pSrc);
    static void     AddVectors(Vector2D *pDst, const Vector2D *pSrc1, const Vector2D *pSrc2);
    static void     SubVectors(Vector2D *pDst, const Vector2D *pSrc1, const Vector2D *pSrc2);
    static void     InterpVectors(Vector2D *pDst, const Vector2D *pSrc1, const Vector2D *pSrc2, const float rate);
    static void     SwapVectors(Vector2D *pVector1, Vector2D *pVector2);
    static bool     IntersectVectors(const Vector2D *pVectorA, const Vector2D *pVectorB, const Vector2D *pVectorC, const Vector2D *pVectorD);
    static float    SignVectors(const Vector2D *pVectorA, const Vector2D *pVectorB, const Vector2D *pVectorC);
    static bool     PointInTriangle(const Vector2D *pPoint, const Vector2D *pVectorA, const Vector2D *pVectorB, const Vector2D *pVectorC);
};

#endif
