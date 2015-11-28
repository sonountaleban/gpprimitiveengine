#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

#include "Engine3D/Matrix.h"

class Vector3D
{
public:
    float           m_x;
    float           m_y;
    float           m_z;

    // ------------------------------------------------------------------

    Vector3D();
    Vector3D(const float x, const float y, const float z);
    ~Vector3D();

    void            Set(const float x, const float y, const float z);
    float           GetMagnitude() const ;
    float           GetMagnitude2() const ;

    static void     CopyVector(Vector3D *pDst, const Vector3D *pSrc);
    static void     AddVectors(Vector3D *pDst, const Vector3D *pSrc1, const Vector3D *pSrc2);
    static void     SubVectors(Vector3D *pDst, const Vector3D *pSrc1, const Vector3D *pSrc2);
    static void     ReverseVector(Vector3D *pDst, const Vector3D *pSrc);
    static void     MultiplyVectorScalar(Vector3D *pDst, const Vector3D *pSrc, const float value);
    static void     InterpVectors(Vector3D *pDst, const Vector3D *pSrc1, const Vector3D *pSrc2, const float rate);
    static void     SwapVectors(Vector3D *pVector1, Vector3D *pVector2);
    static void     NormalizeVector(Vector3D *pDst, const Vector3D *pSrc);
    static float    DotProductVectors(const Vector3D *pVector1, const Vector3D *pVector2);
    static void     CrossProductVectors(Vector3D *pDst, const Vector3D *pSrc1, const Vector3D *pSrc2);
    static void     MultiplyVectorMatrix(Vector3D *pDst, const Vector3D *pSrc1, const Matrix *pSrc2);
};

#endif
