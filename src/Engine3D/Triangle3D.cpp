#include "Engine3D/Triangle3D.h"

Triangle3D::Triangle3D(const int textureIndex, const int vertex1, const int vertex2, const int vertex3)
{
    int i;

    m_type = TYPE_TRIANGLE3D;

    m_numVertexes = 3;
    m_numMaxVertexesClipped = 6;
    m_numVertexesClipped = 0;

    m_pListNumVertexes = new int[m_numVertexes];
    m_pListNumVertexes[0] = vertex1;
    m_pListNumVertexes[1] = vertex2;
    m_pListNumVertexes[2] = vertex3;

    m_pListNumVertexesCopy = new int[m_numVertexes];
    m_pListNumVertexesCopy[0] = vertex1;
    m_pListNumVertexesCopy[1] = vertex2;
    m_pListNumVertexesCopy[2] = vertex3;

    m_ppListVertexesProcessed = new ExtVector3D *[m_numVertexes];
    for (i = 0; i < m_numVertexes; i ++)
    {
        m_ppListVertexesProcessed[i] = NULL;
    }

    m_ppListVertexesClipped = new ExtVector3D *[m_numMaxVertexesClipped];
    for (i = 0; i < m_numMaxVertexesClipped; i ++)
    {
        m_ppListVertexesClipped[i] = NULL;
    }

    m_culling = true;

    m_textureIndex = textureIndex;
}

Triangle3D::~Triangle3D()
{
}

bool Triangle3D::IsVisible()
{
    bool        ok = false;
    int         i;
    float       c;
    ExtVector3D *pVertex1;
    ExtVector3D *pVertex2;
    ExtVector3D *pVertex3;

    for (i = 0; i < m_numVertexes; i ++)
    {
        if (m_ppListVertexesProcessed[i] && m_ppListVertexesProcessed[i]->m_trasformTo2D)
        {
            ok = true;

            break;
        }
    }

    if (ok && m_culling)
    {
        pVertex1 = m_ppListVertexesProcessed[0];
        pVertex2 = m_ppListVertexesProcessed[1];
        pVertex3 = m_ppListVertexesProcessed[2];

        c = pVertex3->m_pos.m_x * ((pVertex1->m_pos.m_z * pVertex2->m_pos.m_y) - (pVertex1->m_pos.m_y * pVertex2->m_pos.m_z)) +
                pVertex3->m_pos.m_y * ((pVertex1->m_pos.m_x * pVertex2->m_pos.m_z) - (pVertex1->m_pos.m_z * pVertex2->m_pos.m_x)) +
                pVertex3->m_pos.m_z * ((pVertex1->m_pos.m_y * pVertex2->m_pos.m_x) - (pVertex1->m_pos.m_x * pVertex2->m_pos.m_y));
        if (c >= 0.0f)
        {
            ok = false;
        }
    }

    return ok;
}

void Triangle3D::AddFirstVertexClipped()
{
    if (m_numVertexesClipped < m_numMaxVertexesClipped)
    {
        m_ppListVertexesClipped[m_numVertexesClipped ++] = m_ppListVertexesClipped[0];
    }
}

