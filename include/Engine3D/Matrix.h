#ifndef _MATRIX_H_
#define _MATRIX_H_

class Matrix
{
public:
    float       m_matrix[9];

    // ------------------------------------------------------------------

    Matrix();
    ~Matrix();

    void        SetIdentity();
    void        Rotate(float angleX, float angleY, float angleZ);
    void        Scale(float x, float y, float z);

    static void CopyMatrix(Matrix *pDst, const Matrix *pSrc);
    static void MultiplyMatrices(Matrix *pDst, const Matrix *pSrc1, const Matrix *pSrc2);
    static void InvertMatrix(Matrix *pDst, const Matrix *pSrc);
};

#endif
