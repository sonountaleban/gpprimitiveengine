#include "Engine3D/Primitive3D.h"

Primitive3D::~Primitive3D()
{
    if (m_pListNumVertexes)
    {
        delete[] m_pListNumVertexes;
    }

    if (m_pListNumVertexesCopy)
    {
        delete[] m_pListNumVertexesCopy;
    }

    if (m_ppListVertexesProcessed)
    {
        delete[] m_ppListVertexesProcessed;
    }

    if (m_ppListVertexesClipped)
    {
        delete[] m_ppListVertexesClipped;
    }
}

bool Primitive3D::IsVisible()
{
    int i;

    for (i = 0; i < m_numVertexes; i ++)
    {
        if (m_ppListVertexesProcessed[i] && m_ppListVertexesProcessed[i]->m_trasformTo2D)
        {
            return true;
        }
    }

    return false;
}

Primitive3D::Types Primitive3D::GetType()
{
    return m_type;
}

void Primitive3D::AddOffsetToListNumVertexes(int offset)
{
    int a;

    for (a = 0; a < m_numVertexes; a ++)
    {
        m_pListNumVertexes[a] += offset;
    }
}

void Primitive3D::ResetListNumVertexes()
{
    int a;

    for (a = 0; a < m_numVertexes; a ++)
    {
        m_pListNumVertexes[a] = m_pListNumVertexesCopy[a];
    }
}

void Primitive3D::AddVertexClipped(ExtVector3D *pVertex)
{
    if (pVertex && m_numVertexesClipped < m_numMaxVertexesClipped)
    {
        m_ppListVertexesClipped[m_numVertexesClipped ++] = pVertex;
    }
}

void Primitive3D::RemoveAllVertexesClipped()
{
    int i;

    for (i = 0; i < m_numVertexesClipped; i ++)
    {
        m_ppListVertexesClipped[i] = NULL;
    }
    m_numVertexesClipped = 0;
}

void Primitive3D::TrasformAllVertexesClippedTo2D(const int screenCenterX, const int screenCenterY, const float screenDistX, const float screenDistY)
{
    int i;

    for (i = 0; i < m_numVertexesClipped; i ++)
    {
        m_ppListVertexesClipped[i]->TrasformTo2D(screenCenterX, screenCenterY, screenDistX, screenDistY);
    }
}
