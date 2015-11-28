#include "utils.h"
#include "Engine3D/Vector3D.h"

Vector3D::Vector3D()
{
    Set(0.0f, 0.0f, 0.0f);
}

Vector3D::Vector3D(const float x, const float y, const float z)
{
    Set(x, y, z);
}

Vector3D::~Vector3D()
{
}

void Vector3D::Set(const float x, const float y, const float z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

float Vector3D::GetMagnitude() const
{
    return sqrt(m_x * m_x + m_y * m_y +  m_z * m_z);
}

float Vector3D::GetMagnitude2() const
{
    return m_x * m_x + m_y * m_y + m_z * m_z;
}

void Vector3D::CopyVector(Vector3D *pDst, const Vector3D *pSrc)
{
    if (pSrc && pDst)
    {
        pDst->m_x = pSrc->m_x;
        pDst->m_y = pSrc->m_y;
        pDst->m_z = pSrc->m_z;
    }
}

void Vector3D::AddVectors(Vector3D *pDst, const Vector3D *pSrc1, const Vector3D *pSrc2)
{
    if (pSrc1 && pSrc2 && pDst)
    {
        pDst->m_x = pSrc1->m_x + pSrc2->m_x;
        pDst->m_y = pSrc1->m_y + pSrc2->m_y;
        pDst->m_z = pSrc1->m_z + pSrc2->m_z;
    }
}

void Vector3D::SubVectors(Vector3D *pDst, const Vector3D *pSrc1, const Vector3D *pSrc2)
{
    if (pSrc1 && pSrc2 && pDst)
    {
        pDst->m_x = pSrc1->m_x - pSrc2->m_x;
        pDst->m_y = pSrc1->m_y - pSrc2->m_y;
        pDst->m_z = pSrc1->m_z - pSrc2->m_z;
    }
}

void Vector3D::ReverseVector(Vector3D *pDst, const Vector3D *pSrc)
{
    if (pSrc && pDst)
    {
        pDst->m_x = -pSrc->m_x;
        pDst->m_y = -pSrc->m_y;
        pDst->m_z = -pSrc->m_z;
    }
}

void Vector3D::MultiplyVectorScalar(Vector3D *pDst, const Vector3D *pSrc, const float value)
{
    if (pSrc && pDst)
    {
        pDst->m_x = pSrc->m_x * value;
        pDst->m_y = pSrc->m_y * value;
        pDst->m_z = pSrc->m_z * value;
    }
}

void Vector3D::InterpVectors(Vector3D *pDst, const Vector3D *pSrc1, const Vector3D *pSrc2, const float rate)
{
    if (pSrc1 && pSrc2 && pDst)
    {
        pDst->m_x = pSrc1->m_x + (pSrc2->m_x - pSrc1->m_x) * rate;
        pDst->m_y = pSrc1->m_y + (pSrc2->m_y - pSrc1->m_y) * rate;
        pDst->m_z = pSrc1->m_z + (pSrc2->m_z - pSrc1->m_z) * rate;
    }
}

void Vector3D::SwapVectors(Vector3D *pVector1, Vector3D *pVector2)
{
    float x;
    float y;
    float z;

    if (!pVector1 || !pVector2)
    {
        return;
    }

    x = pVector1->m_x;
    y = pVector1->m_y;
    z = pVector1->m_z;

    pVector1->m_x = pVector2->m_x;
    pVector1->m_y = pVector2->m_y;
    pVector1->m_z = pVector2->m_z;

    pVector2->m_x = x;
    pVector2->m_y = y;
    pVector2->m_z = z;
}

void Vector3D::NormalizeVector(Vector3D *pDst, const Vector3D *pSrc)
{
    float magnitude;

    if (pSrc && pDst)
    {
        magnitude = pSrc->GetMagnitude();
        if (magnitude <= TOLLERANCE)
        {
            magnitude = 1.0f;
        }

        pDst->m_x = pSrc->m_x / magnitude;
        pDst->m_y = pSrc->m_y / magnitude;
        pDst->m_z = pSrc->m_z / magnitude;
    }
}

float Vector3D::DotProductVectors(const Vector3D *pVector1, const Vector3D *pVector2)
{
    if (pVector1 && pVector2)
    {
        return pVector1->m_x * pVector2->m_x + pVector1->m_y * pVector2->m_y + pVector1->m_z * pVector2->m_z;
    }
    else
    {
        return 0.0f;
    }
}

void Vector3D::CrossProductVectors(Vector3D *pDst, const Vector3D *pSrc1, const Vector3D *pSrc2)
{
    if (pSrc1 && pSrc2 && pDst)
    {
        pDst->m_x = pSrc1->m_y * pSrc2->m_z - pSrc1->m_z * pSrc2->m_y;
        pDst->m_y = pSrc1->m_z * pSrc2->m_x - pSrc1->m_x * pSrc2->m_z;
        pDst->m_z = pSrc1->m_x * pSrc2->m_y - pSrc1->m_y * pSrc2->m_x;
    }
}

void Vector3D::MultiplyVectorMatrix(Vector3D *pDst, const Vector3D *pSrc1, const Matrix *pSrc2)
{
    if (pSrc1 && pSrc2 && pDst)
    {
        pDst->m_x = pSrc1->m_x * pSrc2->m_matrix[0] + pSrc1->m_y * pSrc2->m_matrix[1] + pSrc1->m_z * pSrc2->m_matrix[2];
        pDst->m_y = pSrc1->m_x * pSrc2->m_matrix[3] + pSrc1->m_y * pSrc2->m_matrix[4] + pSrc1->m_z * pSrc2->m_matrix[5];
        pDst->m_z = pSrc1->m_x * pSrc2->m_matrix[6] + pSrc1->m_y * pSrc2->m_matrix[7] + pSrc1->m_z * pSrc2->m_matrix[8];
    }
}
