#include "utils.h"
#include "Engine3D/Line3D.h"

Line3D::Line3D(const int vertex1, const int vertex2)
{
    int i;

    m_type = TYPE_LINE3D;

    m_numVertexes = 2;
    m_numMaxVertexesClipped = 2;
    m_numVertexesClipped = 0;

    m_pListNumVertexes = new int[m_numVertexes];
    m_pListNumVertexes[0] = vertex1;
    m_pListNumVertexes[1] = vertex2;

    m_pListNumVertexesCopy = new int[m_numVertexes];
    m_pListNumVertexesCopy[0] = vertex1;
    m_pListNumVertexesCopy[1] = vertex2;

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
}

Line3D::~Line3D()
{
}

