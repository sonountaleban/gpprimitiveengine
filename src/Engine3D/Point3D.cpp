#include "utils.h"
#include "Engine3D/Point3D.h"

Point3D::Point3D(const int vertex)
{
    int i;

    m_type = TYPE_POINT3D;

    m_numVertexes = 1;
    m_numMaxVertexesClipped = 1;
    m_numVertexesClipped = 0;

    m_pListNumVertexes = new int[m_numVertexes];
    m_pListNumVertexes[0] = vertex;

    m_pListNumVertexesCopy = new int[m_numVertexes];
    m_pListNumVertexesCopy[0] = vertex;

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

Point3D::~Point3D()
{
}

