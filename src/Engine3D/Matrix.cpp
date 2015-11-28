#include "utils.h"
#include "Engine3D/Matrix.h"

Matrix::Matrix()
{
    memset(m_matrix, 0, sizeof(float) * 9);
}

Matrix::~Matrix()
{
}

void Matrix::CopyMatrix(Matrix *pDst, const Matrix *pSrc)
{
    if (pSrc && pDst)
    {
        memcpy(pDst->m_matrix, pSrc->m_matrix, sizeof(float) * 9);
    }
}

void Matrix::MultiplyMatrices(Matrix *pDst, const Matrix *pSrc1, const Matrix *pSrc2)
{
    int x;
    int y;
    int offsetY;

    if (pSrc1 && pSrc2 && pDst)
    {
        for (x = 0; x < 3; x ++)
        {
            for (y = 0, offsetY = 0; y < 3; y ++, offsetY += 3)
            {
                pDst->m_matrix[x + offsetY] = pSrc1->m_matrix[x] * pSrc2->m_matrix[offsetY] +
                    pSrc1->m_matrix[x + 3] * pSrc2->m_matrix[offsetY + 1] +
                    pSrc1->m_matrix[x + 6] * pSrc2->m_matrix[offsetY + 2];
            }
        }
    }
}

void Matrix::InvertMatrix(Matrix *pDst, const Matrix *pSrc)
{
    int i;
    int offsetX = 0;
    int offsetY = 0;

    if (pSrc && pDst)
    {
        for (i = 0; i < 9; i ++, offsetY += 3)
        {
            if (offsetY > 6)
            {
                offsetX ++;
                offsetY = 0;
            }
            pDst->m_matrix[i] = pSrc->m_matrix[offsetX + offsetY];
        }
    }
}

void Matrix::SetIdentity()
{
    m_matrix[0] = 1.0f;
    m_matrix[3] = 0.0f;
    m_matrix[6] = 0.0f;
    m_matrix[1] = 0.0f;
    m_matrix[4] = 1.0f;
    m_matrix[7] = 0.0f;
    m_matrix[2] = 0.0f;
    m_matrix[5] = 0.0f;
    m_matrix[8] = 1.0f;
}

void Matrix::Rotate(float angleX, float angleY, float angleZ)
{
    float   sAngleX;
    float   cAngleX;
    float   sAngleY;
    float   cAngleY;
    float   sAngleZ;
    float   cAngleZ;
    Matrix  matrixTemp1;
    Matrix  matrixTemp2;
    Matrix  matrixTempX;
    Matrix  matrixTempY;
    Matrix  matrixTempZ;

    angleX = TO_RADIANS(angleX);
    angleY = TO_RADIANS(angleY);
    angleZ = TO_RADIANS(angleZ);

    sAngleX = sinf(angleX);
    cAngleX = cosf(angleX);
    sAngleY = sinf(angleY);
    cAngleY = cosf(angleY);
    sAngleZ = sinf(angleZ);
    cAngleZ = cosf(angleZ);

    matrixTempX.SetIdentity();
    matrixTempX.m_matrix[4] = cAngleX;
    matrixTempX.m_matrix[5] = sAngleX;
    matrixTempX.m_matrix[7] = -sAngleX;
    matrixTempX.m_matrix[8] = cAngleX;

    matrixTempY.SetIdentity();
    matrixTempY.m_matrix[0] = cAngleY;
    matrixTempY.m_matrix[2] = -sAngleY;
    matrixTempY.m_matrix[6] = sAngleY;
    matrixTempY.m_matrix[8] = cAngleY;

    matrixTempZ.SetIdentity();
    matrixTempZ.m_matrix[0] = cAngleZ;
    matrixTempZ.m_matrix[1] = sAngleZ;
    matrixTempZ.m_matrix[3] = -sAngleZ;
    matrixTempZ.m_matrix[4] = cAngleZ;

    MultiplyMatrices(&matrixTemp1, this, &matrixTempZ);
    MultiplyMatrices(&matrixTemp2, &matrixTemp1, &matrixTempY);
    MultiplyMatrices(this, &matrixTemp2, &matrixTempX);
}

void Matrix::Scale(float x, float y, float z)
{
    m_matrix[0] = x;
    m_matrix[4] = y;
    m_matrix[8] = z;
}
