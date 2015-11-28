#include "Engine3D/Engine3D.h"
#include "Engine3D/Triangle3D.h"
#include "Engine3D/Point3D.h"
#include "Engine3D/Line3D.h"
#include "TexturesManager.h"

Engine3D *g_pEngine3D = NULL;

Engine3D::Engine3D(const Color *pBackgroundColor, const ShadingTypes shading)
{
    m_pMemoryPoolVertexes = new MemoryPool(1024 * 10000);

    if (pBackgroundColor)
    {
        m_backgroundColor.Set(pBackgroundColor->m_red, pBackgroundColor->m_green, pBackgroundColor->m_blue, 255);
    }

    m_shading = shading;

    Reset();
}

Engine3D::~Engine3D()
{
    Reset();

    if (m_pMemoryPoolVertexes)
    {
        delete m_pMemoryPoolVertexes;
    }
}

void Engine3D::Reset()
{
    m_numVertexesClipped = 0;

    ClearListVertexes();
    ClearListPrimitives();

    m_pCamera = NULL;

    m_pLightAmbient = NULL;
    m_pLightOther = NULL;
}

bool Engine3D::AddVertex3D(ExtVector3D *pVertex)
{
    int     index;
    Element e;

    if (pVertex)
    {
        e.p = pVertex;

        index = m_listVertexesOriginal.AddElement(e);
        if (index >= 0)
        {
            //e.p = new ExtVector3D();
            e.p = new (m_pMemoryPoolVertexes->Allocate(sizeof(ExtVector3D), 4)) ExtVector3D();

            if (m_listVertexesProcessed.AddElement(e) >= 0)
            {
                return true;
            }
            else
            {
                //delete (ExtVector3D *)e.p;

                m_listVertexesOriginal.RemoveElement(index);
            }
        }
    }

    return false;
}

int Engine3D::GetNumVertexes3D()
{
    return m_listVertexesOriginal.GetNumElements();
}

bool Engine3D::RemoveVertex3D(ExtVector3D *pVertex)
{
    Element e;

    if (pVertex)
    {
        e.p = pVertex;

        if (m_listVertexesOriginal.RemoveElement(e))
        {
            e = m_listVertexesProcessed.GetElement(0);

            if (e.p)
            {
                //delete (ExtVector3D *)e.p;
                ((ExtVector3D *)e.p)->~ExtVector3D();
            }

            m_listVertexesProcessed.RemoveElement(0);

            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

bool Engine3D::AddPrimitive3D(Primitive3D *pPrimitive)
{
    int     i;
    Element e;

    if (pPrimitive)
    {
        e.p = pPrimitive;

        if (m_listPrimitives.AddElement(e) >= 0)
        {
            for (i = 0; i < pPrimitive->m_numVertexes; i ++)
            {
                e = m_listVertexesProcessed.GetElement(pPrimitive->m_pListNumVertexes[i]);

                pPrimitive->m_ppListVertexesProcessed[i] = (ExtVector3D *)e.p;
            }

            return true;
        }
    }

    return false;
}

int Engine3D::GetNumPrimitives3D()
{
    return m_listPrimitives.GetNumElements();
}

bool Engine3D::RemovePrimitive3D(Primitive3D *pPrimitive)
{
    Element e;

    if (pPrimitive)
    {
        e.p = pPrimitive;

        return m_listPrimitives.RemoveElement(e);
    }

    return false;
}

void Engine3D::AddLight(Light *pLight)
{
    if (!pLight || pLight->m_type == Light::TYPE_NONE)
    {
        m_pLightOther = NULL;
    }
    else if (pLight->m_type == Light::TYPE_AMBIENT)
    {
        m_pLightAmbient = pLight;
    }
    else
    {
        m_pLightOther = pLight;
    }
}

void Engine3D::SetCamera(Camera *pCamera)
{
    m_pCamera = pCamera;
}

void Engine3D::ClearBackground()
{
    int i;
    int size;

    if (m_pCamera)
    {
        size = m_pCamera->m_screenWidth * m_pCamera->m_screenHeight;

        for (i = 0; i < size; i ++)
        {
            m_pCamera->m_pScreenBuffer[i] = m_backgroundColor.m_rgba;
        }
    }
}

void Engine3D::ClearZBuffer()
{
    if (m_pCamera)
    {
        m_pCamera->ClearZBuffer();
    }
}

void Engine3D::ClearListVertexes()
{
    int     a;
    Element e;

    m_listVertexesOriginal.RemoveAllElements();

    for (a = 0; a < m_listVertexesProcessed.GetNumElements(); a ++)
    {
        e = m_listVertexesProcessed.GetElement(a);
        if (e.p)
        {
            //delete (ExtVector3D *)e.p;
            ((ExtVector3D *)e.p)->~ExtVector3D();
        }
    }
    m_listVertexesProcessed.RemoveAllElements();

    for (a = 0; a < m_listVertexesClipped.GetNumElements(); a ++)
    {
        e = m_listVertexesClipped.GetElement(a);
        if (e.p)
        {
            //delete (ExtVector3D *)e.p;
            ((ExtVector3D *)e.p)->~ExtVector3D();
        }
    }
    m_listVertexesClipped.RemoveAllElements();

    if (m_pMemoryPoolVertexes)
    {
        m_pMemoryPoolVertexes->Reset();
    }
}

void Engine3D::ClearListPrimitives()
{
    m_listPrimitives.RemoveAllElements();
}

void Engine3D::Render()
{
    int         a;
    int         diff;
    Element     e;
    Vector3D    vertexTemp;
    ExtVector3D *pVertexOriginal;
    ExtVector3D *pVertexProcessed;

    if (!m_pCamera)
    {
        return;
    }

    if (m_listVertexesClipped.GetNumElements() < m_listVertexesOriginal.GetNumElements() * 3)
    {
        diff = m_listVertexesOriginal.GetNumElements() * 3 - m_listVertexesClipped.GetNumElements();
        for (a = 0; a < diff; a ++)
        {
            //e.p = new ExtVector3D();
            e.p = new (m_pMemoryPoolVertexes->Allocate(sizeof(ExtVector3D), 4)) ExtVector3D();

            m_listVertexesClipped.AddElement(e);
        }
    }

    /*for (a = 0; a < m_listVertexesClipped.GetNumElements(); a ++)
    {
        e = m_listVertexesClipped.GetElement(a);
        if (e.p)
        {
            ((ExtVector3D *)e.p)->Reset();
        }
    }*/

    CalcNormalsAndGouraudShading();

    for (a = 0; a < m_listVertexesProcessed.GetNumElements(); a ++)
    {
        e = m_listVertexesOriginal.GetElement(a);
        if (e.p)
        {
            pVertexOriginal = (ExtVector3D *)e.p;
        }
        else
        {
            pVertexOriginal = NULL;
        }

        e = m_listVertexesProcessed.GetElement(a);
        if (e.p)
        {
            pVertexProcessed = (ExtVector3D *)e.p;
        }
        else
        {
            pVertexProcessed = NULL;
        }

        if (pVertexOriginal && pVertexProcessed)
        {
            pVertexProcessed->Reset();

            vertexTemp.Set(pVertexOriginal->m_pos.m_x - m_pCamera->m_pos.m_x,
                           pVertexOriginal->m_pos.m_y - m_pCamera->m_pos.m_y,
                           pVertexOriginal->m_pos.m_z - m_pCamera->m_pos.m_z);
            Vector3D::MultiplyVectorMatrix(&pVertexProcessed->m_pos, &vertexTemp, &m_pCamera->m_matrixRot);

            Color::CopyColor(&pVertexProcessed->m_color, &pVertexOriginal->m_colorProcessed);

            Vector2D::CopyVector(&pVertexProcessed->m_uv, &pVertexOriginal->m_uv);

            if (pVertexProcessed->m_pos.m_z >= m_pCamera->m_nearZ && pVertexProcessed->m_pos.m_z <= m_pCamera->m_farZ)
            {
                pVertexProcessed->m_trasformTo2D = true;
            }
        }
    }

    m_numVertexesClipped = 0;

    ClipPoints();

    ClipLines();

    ClipTriangles();

    TrasformVertexes3Dto2D();

    Draw();

    //DEBUGLOG2("%d/%d\n", m_pMemoryPoolVertexes->GetFreeBytes(), m_pMemoryPoolVertexes->GetOriginalSize());
}

void Engine3D::CalcNormalsAndGouraudShading()
{
    int         a;
    int         b;
    int         c;
    int         index1;
    int         numNormals;
    float       ratioColorLightOther;
    Element     e;
    Element     e2;
    Triangle3D  *pTriangle;
    ExtVector3D *pVertex;

    if (m_pLightOther && m_shading == SHADING_TYPE_GOURAUD)
    {
        for (a = 0; a < m_listPrimitives.GetNumElements(); a ++)
        {
            e = m_listPrimitives.GetElement(a);
            if (e.p && ((Primitive3D *)e.p)->GetType() == Primitive3D::TYPE_TRIANGLE3D)
            {
                pTriangle = (Triangle3D *)e.p;
                pTriangle->m_plane.Set(&((ExtVector3D *)m_listVertexesOriginal.GetElement(pTriangle->m_pListNumVertexes[0]).p)->m_pos,
                        &((ExtVector3D *)m_listVertexesOriginal.GetElement(pTriangle->m_pListNumVertexes[1]).p)->m_pos,
                        &((ExtVector3D *)m_listVertexesOriginal.GetElement(pTriangle->m_pListNumVertexes[2]).p)->m_pos);
            }
        }

        for (a = 0, index1 = 0; a < m_listVertexesOriginal.GetNumElements(); a ++, index1 ++)
        {
            e = m_listVertexesOriginal.GetElement(a);
            if (e.p)
            {
                pVertex = (ExtVector3D *)e.p;
                if (pVertex->m_processColor)
                {
                    pVertex->m_normal.Set(0.0f, 0.0f, 0.0f);

                    numNormals = 0;

                    for (b = 0; b < m_listPrimitives.GetNumElements(); b ++)
                    {
                        e2 = m_listPrimitives.GetElement(b);
                        if (e2.p && ((Primitive3D *)e2.p)->GetType() == Primitive3D::TYPE_TRIANGLE3D)
                        {
                            pTriangle = (Triangle3D *)e2.p;

                            for (c = 0; c < pTriangle->m_numVertexes; c ++)
                            {
                                if (pTriangle->m_pListNumVertexes[c] == index1)
                                {
                                    numNormals ++;

                                    Vector3D::AddVectors(&pVertex->m_normal, &pVertex->m_normal, &pTriangle->m_plane.m_normal);

                                    break;
                                }
                            }
                        }
                    }

                    if (numNormals > 0)
                    {
                        Vector3D::MultiplyVectorScalar(&pVertex->m_normal, &pVertex->m_normal, 1.0f / numNormals);
                        Vector3D::NormalizeVector(&pVertex->m_normal, &pVertex->m_normal);

                        if (m_pLightOther->m_type == Light::TYPE_DIRECTIONAL)
                        {
                            ratioColorLightOther = Vector3D::DotProductVectors(&pVertex->m_normal, &m_pLightOther->m_vectorRot);
                        }
                        else if (m_pLightOther->m_type == Light::TYPE_POINT || m_pLightOther->m_type == Light::TYPE_SPOT)
                        {
                            Vector3D::SubVectors(&m_vectorTemp, &m_pLightOther->m_pos, &pVertex->m_pos);
                            Vector3D::NormalizeVector(&m_vectorTemp, &m_vectorTemp);

                            ratioColorLightOther = Vector3D::DotProductVectors(&m_vectorTemp, &pVertex->m_normal);

                            if (m_pLightOther->m_type == Light::TYPE_SPOT)
                            {
                                if (ratioColorLightOther > 0.0f)
                                {
                                    ratioColorLightOther *= Vector3D::DotProductVectors(&m_pLightOther->m_vectorRot, &pVertex->m_normal);
                                }
                                else
                                {
                                    ratioColorLightOther = 0.0f;
                                }
                            }
                        }
                        else
                        {
                            ratioColorLightOther = 0.0f;
                        }

                        if (ratioColorLightOther < 0.0f)
                        {
                            ratioColorLightOther = 0.0f;
                        }

                        if (m_pLightAmbient)
                        {
                            Color::CopyColor(&m_colorTemp, &m_pLightAmbient->m_color);
                        }
                        else
                        {
                            m_colorTemp.Set(0, 0, 0, 255);
                        }

                        Color::InterpColors(&m_colorTemp2, &m_colorTemp, &m_pLightOther->m_color, ratioColorLightOther);
                        Color::MultiplyColors(&pVertex->m_colorProcessed, &pVertex->m_color, &m_colorTemp2);
                    }
                    else
                    {
                        Color::CopyColor(&pVertex->m_colorProcessed, &pVertex->m_color);
                    }
                }
                else
                {
                    Color::CopyColor(&pVertex->m_colorProcessed, &pVertex->m_color);
                }
            }
        }
    }
    else
    {
        for (a = 0, index1 = 0; a < m_listVertexesOriginal.GetNumElements(); a ++, index1 ++)
        {
            e = m_listVertexesOriginal.GetElement(a);
            if (e.p)
            {
                pVertex = (ExtVector3D *)e.p;
                if (pVertex->m_processColor)
                {
                    if (m_pLightAmbient && !m_pLightOther && m_shading == SHADING_TYPE_GOURAUD)
                    {
                        Color::MultiplyColors(&pVertex->m_colorProcessed, &pVertex->m_color, &m_pLightAmbient->m_color);
                    }
                    else
                    {
                        Color::CopyColor(&pVertex->m_colorProcessed, &pVertex->m_color);
                    }
                }
                else
                {
                    Color::CopyColor(&pVertex->m_colorProcessed, &pVertex->m_color);
                }
            }
        }
    }
}

void Engine3D::ClipPoints()
{
    int         a;
    Element     e;
    Point3D     *pPoint;
    ExtVector3D *pVertex;

    for (a = 0; a < m_listPrimitives.GetNumElements(); a ++)
    {
        e = m_listPrimitives.GetElement(a);
        if (e.p && ((Primitive3D *)e.p)->GetType() == Primitive3D::TYPE_POINT3D)
        {
            pPoint = (Point3D *)e.p;
            pPoint->RemoveAllVertexesClipped();

            if (!pPoint->IsVisible())
            {
                continue;
            }

            pVertex = pPoint->m_ppListVertexesProcessed[0];

            //if (pVertex->m_trasformTo2D)
            {
                pPoint->AddVertexClipped(pVertex);
            }
        }
    }
}

void Engine3D::ClipLines()
{
    int         a;
    float       diffDistMinVertexZ;
    float       diffDistMaxVertexZ;
    float       diffVertexesZ;
    float       ratioDiffZ;
    float       distMinZ = m_pCamera->m_nearZ;
    float       distMaxZ = m_pCamera->m_farZ;
    Line3D      *pLine;
    ExtVector3D *pVertexClipped;
    ExtVector3D *pVertex1;
    ExtVector3D *pVertex2;
    Element     e;

    for (a = 0; a < m_listPrimitives.GetNumElements(); a ++)
    {
        e = m_listPrimitives.GetElement(a);
        if (e.p && ((Primitive3D *)e.p)->GetType() == Primitive3D::TYPE_LINE3D)
        {
            pLine = (Line3D *)e.p;
            pLine->RemoveAllVertexesClipped();

            if (!pLine->IsVisible())
            {
                continue;
            }

            pVertex1 = pLine->m_ppListVertexesProcessed[0];
            pVertex2 = pLine->m_ppListVertexesProcessed[1];

            if (pVertex1->m_trasformTo2D && pVertex2->m_trasformTo2D)
            {
                pLine->AddVertexClipped(pVertex1);
                pLine->AddVertexClipped(pVertex2);
            }
            else if (pVertex1->m_trasformTo2D || pVertex2->m_trasformTo2D)
            {
                diffVertexesZ = pVertex2->m_pos.m_z - pVertex1->m_pos.m_z;

                if (pVertex1->m_pos.m_z < distMinZ || pVertex2->m_pos.m_z < distMinZ)
                {
                    diffDistMinVertexZ = distMinZ - pVertex1->m_pos.m_z;
                    ratioDiffZ = diffDistMinVertexZ / diffVertexesZ;

                    if (diffVertexesZ < 0)
                    {
                        Color::InterpColors(&m_colorInterpolated, &pVertex1->m_color, &pVertex2->m_color, ratioDiffZ);
                    }
                    else
                    {
                        Color::InterpColors(&m_colorInterpolated, &pVertex2->m_color, &pVertex1->m_color, 1.0f - ratioDiffZ);
                    }

                    m_colorInterpolated.CalcRGBA();

                    pVertexClipped = (ExtVector3D *)m_listVertexesClipped.GetElement(m_numVertexesClipped ++).p;
                    pVertexClipped->m_pos.Set(pVertex1->m_pos.m_x + (pVertex2->m_pos.m_x - pVertex1->m_pos.m_x) * ratioDiffZ,
                            pVertex1->m_pos.m_y + (pVertex2->m_pos.m_y - pVertex1->m_pos.m_y) * ratioDiffZ,
                            distMinZ);
                    pVertexClipped->m_trasformTo2D = true;

                    Color::CopyColor(&pVertexClipped->m_color, &m_colorInterpolated);

                    if (pVertex1->m_pos.m_z >= distMinZ && pVertex2->m_pos.m_z < distMinZ)
                    {
                        pLine->AddVertexClipped(pVertex1);
                        pLine->AddVertexClipped(pVertexClipped);
                    }
                    else
                    {
                        pLine->AddVertexClipped(pVertexClipped);
                        pLine->AddVertexClipped(pVertex2);
                    }
                }
                else
                {
                    diffDistMaxVertexZ = pVertex2->m_pos.m_z - distMaxZ;
                    ratioDiffZ = diffDistMaxVertexZ / diffVertexesZ;

                    if (diffVertexesZ >= 0)
                    {
                        Color::InterpColors(&m_colorInterpolated, &pVertex1->m_color, &pVertex2->m_color, 1.0f - ratioDiffZ);
                    }
                    else
                    {
                        Color::InterpColors(&m_colorInterpolated, &pVertex2->m_color, &pVertex1->m_color, ratioDiffZ);
                    }

                    m_colorInterpolated.CalcRGBA();

                    pVertexClipped = (ExtVector3D *)m_listVertexesClipped.GetElement(m_numVertexesClipped ++).p;
                    pVertexClipped->m_pos.Set(pVertex1->m_pos.m_x + (pVertex2->m_pos.m_x - pVertex1->m_pos.m_x) * (1.0f - ratioDiffZ),
                            pVertex1->m_pos.m_y + (pVertex2->m_pos.m_y - pVertex1->m_pos.m_y) * (1.0f - ratioDiffZ),
                            distMaxZ);
                    pVertexClipped->m_trasformTo2D = true;

                    Color::CopyColor(&pVertexClipped->m_color, &m_colorInterpolated);

                    if (pVertex1->m_pos.m_z <= distMaxZ && pVertex2->m_pos.m_z > distMaxZ)
                    {
                        pLine->AddVertexClipped(pVertex1);
                        pLine->AddVertexClipped(pVertexClipped);
                    }
                    else
                    {
                        pLine->AddVertexClipped(pVertexClipped);
                        pLine->AddVertexClipped(pVertex2);
                    }
                }
            }
        }
    }
}

void Engine3D::ClipTriangles()
{
    bool        firstVertexes;
    int         a;
    int         i;
    int         numVertex1;
    int         numVertex2;
    float       diffDistMinVertexZ;
    float       diffDistMaxVertexZ;
    float       diffVertexesZ;
    float       ratioDiffZ;
    float       distMinZ = m_pCamera->m_nearZ;
    float       distMaxZ = m_pCamera->m_farZ;
    Triangle3D  *pTriangle;
    ExtVector3D *pVertexClipped;
    ExtVector3D *pVertex1;
    ExtVector3D *pVertex2;
    Element     e;

    for (a = 0; a < m_listPrimitives.GetNumElements(); a ++)
    {
        e = m_listPrimitives.GetElement(a);
        if (e.p && ((Primitive3D *)e.p)->GetType() == Primitive3D::TYPE_TRIANGLE3D)
        {
            pTriangle = (Triangle3D *)e.p;
            pTriangle->RemoveAllVertexesClipped();

            if (!pTriangle->IsVisible())
            {
                continue;
            }

            firstVertexes = true;

            for (i = 0; i < pTriangle->m_numVertexes; i ++)
            {
                numVertex1 = i;
                numVertex2 = numVertex1 + 1;
                if (numVertex2 == pTriangle->m_numVertexes)
                {
                    numVertex2 = 0;
                }

                pVertex1 = pTriangle->m_ppListVertexesProcessed[numVertex1];
                pVertex2 = pTriangle->m_ppListVertexesProcessed[numVertex2];

                if (pVertex1->m_trasformTo2D && pVertex2->m_trasformTo2D)
                {
                    if (firstVertexes)
                    {
                        pTriangle->AddVertexClipped(pVertex1);
                        pTriangle->AddVertexClipped(pVertex2);
                    }
                    else
                    {
                        pTriangle->AddVertexClipped(pVertex2);
                    }

                    firstVertexes = false;
                }
                else if (pVertex1->m_trasformTo2D || pVertex2->m_trasformTo2D)
                {
                    diffVertexesZ = pVertex2->m_pos.m_z - pVertex1->m_pos.m_z;

                    if (pVertex1->m_pos.m_z < distMinZ || pVertex2->m_pos.m_z < distMinZ)
                    {
                        diffDistMinVertexZ = distMinZ - pVertex1->m_pos.m_z;
                        ratioDiffZ = diffDistMinVertexZ / diffVertexesZ;

                        if (diffVertexesZ < 0)
                        {
                            Color::InterpColors(&m_colorInterpolated, &pVertex1->m_color, &pVertex2->m_color, ratioDiffZ);

                            Vector2D::InterpVectors(&m_uvInterpolated, &pVertex1->m_uv, &pVertex2->m_uv, ratioDiffZ);
                        }
                        else
                        {
                            Color::InterpColors(&m_colorInterpolated, &pVertex2->m_color, &pVertex1->m_color, 1.0f - ratioDiffZ);

                            Vector2D::InterpVectors(&m_uvInterpolated, &pVertex2->m_uv, &pVertex1->m_uv, 1.0f - ratioDiffZ);
                        }

                        m_colorInterpolated.CalcRGBA();

                        pVertexClipped = (ExtVector3D *)m_listVertexesClipped.GetElement(m_numVertexesClipped ++).p;
                        pVertexClipped->m_pos.Set(pVertex1->m_pos.m_x + (pVertex2->m_pos.m_x - pVertex1->m_pos.m_x) * ratioDiffZ,
                                pVertex1->m_pos.m_y + (pVertex2->m_pos.m_y - pVertex1->m_pos.m_y) * ratioDiffZ,
                                distMinZ);
                        pVertexClipped->m_trasformTo2D = true;

                        Color::CopyColor(&pVertexClipped->m_color, &m_colorInterpolated);

                        Vector2D::CopyVector(&pVertexClipped->m_uv, &m_uvInterpolated);

                        if (pVertex1->m_pos.m_z >= distMinZ && pVertex2->m_pos.m_z < distMinZ)
                        {
                            if (firstVertexes)
                            {
                                pTriangle->AddVertexClipped(pVertex1);
                                pTriangle->AddVertexClipped(pVertexClipped);
                            }
                            else
                            {
                                pTriangle->AddVertexClipped(pVertexClipped);
                                if (numVertex2 == 0)
                                {
                                    pTriangle->AddFirstVertexClipped();
                                }
                            }
                        }
                        else
                        {
                            pTriangle->AddVertexClipped(pVertexClipped);
                            pTriangle->AddVertexClipped(pVertex2);
                        }

                        firstVertexes = false;
                    }
                    else
                    {
                        diffDistMaxVertexZ = pVertex2->m_pos.m_z - distMaxZ;
                        ratioDiffZ = diffDistMaxVertexZ / diffVertexesZ;

                        if (diffVertexesZ >= 0)
                        {
                            Color::InterpColors(&m_colorInterpolated, &pVertex1->m_color, &pVertex2->m_color, 1.0f - ratioDiffZ);

                            Vector2D::InterpVectors(&m_uvInterpolated, &pVertex1->m_uv, &pVertex2->m_uv, 1.0f - ratioDiffZ);
                        }
                        else
                        {
                            Color::InterpColors(&m_colorInterpolated, &pVertex2->m_color, &pVertex1->m_color, ratioDiffZ);

                            Vector2D::InterpVectors(&m_uvInterpolated, &pVertex2->m_uv, &pVertex1->m_uv, ratioDiffZ);
                        }

                        m_colorInterpolated.CalcRGBA();

                        pVertexClipped = (ExtVector3D *)m_listVertexesClipped.GetElement(m_numVertexesClipped ++).p;
                        pVertexClipped->m_pos.Set(pVertex1->m_pos.m_x + (pVertex2->m_pos.m_x - pVertex1->m_pos.m_x) * (1.0f - ratioDiffZ),
                                pVertex1->m_pos.m_y + (pVertex2->m_pos.m_y - pVertex1->m_pos.m_y) * (1.0f - ratioDiffZ),
                                distMaxZ);
                        pVertexClipped->m_trasformTo2D = true;

                        Color::CopyColor(&pVertexClipped->m_color, &m_colorInterpolated);

                        Vector2D::CopyVector(&pVertexClipped->m_uv, &m_uvInterpolated);

                        if (pVertex1->m_pos.m_z <= distMaxZ && pVertex2->m_pos.m_z > distMaxZ)
                        {
                            if (firstVertexes)
                            {
                                pTriangle->AddVertexClipped(pVertex1);
                                pTriangle->AddVertexClipped(pVertexClipped);
                            }
                            else
                            {
                                pTriangle->AddVertexClipped(pVertexClipped);
                                if (numVertex2 == 0)
                                {
                                    pTriangle->AddFirstVertexClipped();
                                }
                            }
                        }
                        else
                        {
                            pTriangle->AddVertexClipped(pVertexClipped);
                            pTriangle->AddVertexClipped(pVertex2);
                        }

                        firstVertexes = false;
                    }
                }
            }
        }
    }
}

void Engine3D::TrasformVertexes3Dto2D()
{
    /*int         a;
    Element     e;
    ExtVector3D *pVertex;

    for (a = 0; a < m_listVertexesProcessed.GetNumElements(); a ++)
    {
        e = m_listVertexesProcessed.GetElement(a);
        if (e.p)
        {
            pVertex = (ExtVector3D *)e.p;
            pVertex->TrasformTo2D(m_pCamera->m_screenCenterX, m_pCamera->m_screenCenterY,
                                  m_pCamera->m_screenDistX, m_pCamera->m_screenDistY);
        }
    }

    for (a = 0; a < m_numVertexesClipped; a ++)
    {
        e = m_listVertexesClipped.GetElement(a);
        if (e.p)
        {
            pVertex = (ExtVector3D *)e.p;
            pVertex->TrasformTo2D(m_pCamera->m_screenCenterX, m_pCamera->m_screenCenterY,
                                  m_pCamera->m_screenDistX, m_pCamera->m_screenDistY);
        }
    }*/
    int     a;
    Element e;

    for (a = 0; a < m_listPrimitives.GetNumElements(); a ++)
    {
        e = m_listPrimitives.GetElement(a);
        if (e.p)
        {
            ((Primitive3D *)e.p)->TrasformAllVertexesClippedTo2D(m_pCamera->m_screenCenterX, m_pCamera->m_screenCenterY,
                                                                 m_pCamera->m_screenDistX, m_pCamera->m_screenDistY);
        }
    }
}

void Engine3D::Draw()
{
    int         a;
    int         numTrianglesShowed = 0;
    Point3D     *pPoint;
    Line3D      *pLine;
    Triangle3D  *pTriangle;
    Element     e;

    for (a = 0; a < m_listPrimitives.GetNumElements(); a ++)
    {
        e = m_listPrimitives.GetElement(a);
        if (e.p)
        {
            if (((Primitive3D *)e.p)->GetType() == Primitive3D::TYPE_POINT3D)
            {
                pPoint = (Point3D *)e.p;
                if (pPoint->m_numVertexesClipped > 0)
                {
                    DrawPoint3D(pPoint);
                }
            }
            else if (((Primitive3D *)e.p)->GetType() == Primitive3D::TYPE_LINE3D)
            {
                pLine = (Line3D *)e.p;
                if (pLine->m_numVertexesClipped > 0)
                {
                    DrawLine3D(pLine);
                }
            }
            else if (((Primitive3D *)e.p)->GetType() == Primitive3D::TYPE_TRIANGLE3D)
            {
                pTriangle = (Triangle3D *)e.p;
                if (pTriangle->m_numVertexesClipped > 0)
                {
                    if (DrawTriangle3D(pTriangle))
                    {
                        numTrianglesShowed ++;
                    }
                }
            }
        }
    }

    //DEBUGLOG2("%d\n", numTrianglesShowed);
}

void Engine3D::DrawPoint3D(Point3D *pPoint)
{
    int         x;
    int         y;
    int         offset;
    ExtVector3D *pVertex;

    pVertex = pPoint->m_ppListVertexesClipped[0];

    x = pVertex->m_xOnScreen;
    y = pVertex->m_yOnScreen;

    if (x < 0 || y < 0 || x >= m_pCamera->m_screenWidth || y >= m_pCamera->m_screenHeight)
    {
        return;
    }

    offset = x + y * m_pCamera->m_screenWidth;

    if (m_pCamera->SetValueZBuffer(offset, pVertex->m_pos.m_z, true))
    {
        if (pVertex->m_color.m_alpha == 255)
        {
            m_pCamera->m_pScreenBuffer[offset] = pVertex->m_color.m_rgba;
        }
        else
        {
            Color oldColor;

            oldColor.m_rgba = m_pCamera->m_pScreenBuffer[offset];
            oldColor.CalcRedGreenBlueAlpha();
            oldColor.Blend(&pVertex->m_color);

            m_pCamera->m_pScreenBuffer[offset] = oldColor.m_rgba;
        }
    }
}

void Engine3D::DrawLine3D(Line3D *pLine)
{
    bool        step;
    int         l;
    int         deltaX;
    int         deltaY;
    int         error;
    int         x;
    int         y;
    int         x0;
    int         y0;
    int         x1;
    int         y1;
    int         stepY;
    int         firstIndex = 0;
    int         secondIndex = 1;
    int         temp;
    float       a;
    float       b;
    float       c;
    float       d;
    float       e;
    float       ax;
    float       ay;
    float       bx;
    float       by;
    float       rate;
    float       numer;
    float       denom;
    Vector2D    vectorA;
    Vector2D    vectorB;
    Vector2D    vectorC;
    Vector2D    vectorD;

    x0 = pLine->m_ppListVertexesClipped[firstIndex]->m_xOnScreen;
    y0 = pLine->m_ppListVertexesClipped[firstIndex]->m_yOnScreen;
    x1 = pLine->m_ppListVertexesClipped[secondIndex]->m_xOnScreen;
    y1 = pLine->m_ppListVertexesClipped[secondIndex]->m_yOnScreen;

    if ((x0 < 0 || x0 >= m_pCamera->m_screenWidth || y0 < 0 || y0 >= m_pCamera->m_screenHeight) &&
            (x1 < 0 || x1 >= m_pCamera->m_screenWidth || y1 < 0 || y1 >= m_pCamera->m_screenHeight))
    {
        if ((x0 < 0 && x1 < 0) || (y0 < 0 && y1 < 0) ||
                (x0 >= m_pCamera->m_screenWidth && x1 >= m_pCamera->m_screenWidth) || (y0 >= m_pCamera->m_screenHeight && y1 >= m_pCamera->m_screenHeight))
        {
            return;
        }

        vectorC.Set(x0, y0);
        vectorD.Set(x1, y1);

        vectorA.Set(0.0f, 0.0f);
        vectorB.Set(m_pCamera->m_screenWidth - 1.0f, 0.0f);
        if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
        {
            goto _exit1;
        }

        vectorA.Set(m_pCamera->m_screenWidth - 1.0f, 0.0f);
        vectorB.Set(m_pCamera->m_screenWidth - 1.0f, m_pCamera->m_screenHeight - 1.0f);
        if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
        {
            goto _exit1;
        }

        vectorA.Set(m_pCamera->m_screenWidth - 1.0f, m_pCamera->m_screenHeight - 1.0f);
        vectorB.Set(0, m_pCamera->m_screenHeight - 1.0f);
        if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
        {
            goto _exit1;
        }

        vectorA.Set(0.0f, m_pCamera->m_screenHeight - 1.0f);
        vectorB.Set(0.0f, 0.0f);
        if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
        {
            goto _exit1;
        }

        return;

_exit1:
        ;
    }

    if (abs(x0 - x1) <= 1 && abs(y0 - y1) <= 1)
    {
        if (x0 >= 0 && y0 >= 0 && x0 < m_pCamera->m_screenWidth && y0 < m_pCamera->m_screenHeight)
        {
            DrawLinePoint3D(pLine, x0 + y0 * m_pCamera->m_screenWidth, 0.0f,
                            pLine->m_ppListVertexesClipped[firstIndex]->m_pos.m_z, firstIndex, secondIndex);
        }

        if ((x0 != x1 || y0 != y1) && x1 >= 0 && y1 >= 0 && x1 < m_pCamera->m_screenWidth && y1 < m_pCamera->m_screenHeight)
        {
            DrawLinePoint3D(pLine, x1 + y1 * m_pCamera->m_screenWidth, 1.0f,
                            pLine->m_ppListVertexesClipped[secondIndex]->m_pos.m_z, firstIndex, secondIndex);
        }

        return;
    }

    if (abs(y1 - y0) > abs(x1 - x0))
    {
        step = true;
    }
    else
    {
        step = false;
    }
    if (step)
    {
        l = y0;
        y0 = x0;
        x0 = l;

        l = y1;
        y1 = x1;
        x1 = l;
    }
    if (x1 < x0)
    {
        l = x1;
        x1 = x0;
        x0 = l;

        l = y1;
        y1 = y0;
        y0 = l;

        temp = firstIndex;
        firstIndex = secondIndex;
        secondIndex = temp;
    }

    deltaX = x1 - x0;
    deltaY = abs(y1 - y0);
    error = deltaX >> 1;

    if (y0 < y1)
    {
        stepY = 1;
    }
    else
    {
        stepY = -1;
    }

    Vector3D::SubVectors(&m_renderA, &pLine->m_ppListVertexesClipped[secondIndex]->m_pos, &pLine->m_ppListVertexesClipped[firstIndex]->m_pos);  // V1

    ax = m_pCamera->m_nearZ / m_pCamera->m_screenDistX;
    ay = m_pCamera->m_nearZ / m_pCamera->m_screenDistY;
    bx = m_pCamera->m_farZ / m_pCamera->m_screenDistX;
    by = m_pCamera->m_farZ / m_pCamera->m_screenDistY;

    y = y0;
    for (x = x0; x <= x1; x ++)
    {
        if (step)
        {
            if (y >= 0 && x >= 0 && y < m_pCamera->m_screenWidth && x < m_pCamera->m_screenHeight)
            {
                m_renderB.Set((y - m_pCamera->m_screenCenterX) * ax, (x - m_pCamera->m_screenCenterY) * ay, m_pCamera->m_nearZ);              // P2

                m_renderC.Set((y - m_pCamera->m_screenCenterX) * bx, (x - m_pCamera->m_screenCenterY) * by, m_pCamera->m_farZ);

                Vector3D::SubVectors(&m_renderC, &m_renderC, &m_renderB);                     // V2

                Vector3D::SubVectors(&m_renderD, &pLine->m_ppListVertexesClipped[firstIndex]->m_pos, &m_renderB);                     // P1 - P2

                a = Vector3D::DotProductVectors(&m_renderD, &m_renderC);
                b = Vector3D::DotProductVectors(&m_renderC, &m_renderA);
                c = Vector3D::DotProductVectors(&m_renderD, &m_renderA);
                d = m_renderC.GetMagnitude2();
                e = m_renderA.GetMagnitude2();

                denom = d * e - b * b;
                if (fabs(denom) <= TOLLERANCE)
                {
                    continue;
                }

                numer = a * b - c * d;

                rate = numer / denom;

                if (rate < 0.0f || rate > 1.0f)
                {
                    continue;
                }

                DrawLinePoint3D(pLine, y + x * m_pCamera->m_screenWidth, rate,
                                pLine->m_ppListVertexesClipped[firstIndex]->m_pos.m_z + rate * m_renderA.m_z, firstIndex, secondIndex);
            }
        }
        else
        {
            if (x >= 0 && y >= 0 && x < m_pCamera->m_screenWidth && y < m_pCamera->m_screenHeight)
            {
                m_renderB.Set((x - m_pCamera->m_screenCenterX) * ax, (y - m_pCamera->m_screenCenterY) * ay, m_pCamera->m_nearZ);              // P2

                m_renderC.Set((x - m_pCamera->m_screenCenterX) * bx, (y - m_pCamera->m_screenCenterY) * by, m_pCamera->m_farZ);

                Vector3D::SubVectors(&m_renderC, &m_renderC, &m_renderB);                     // V2

                Vector3D::SubVectors(&m_renderD, &pLine->m_ppListVertexesClipped[firstIndex]->m_pos, &m_renderB);                     // P1 - P2

                a = Vector3D::DotProductVectors(&m_renderD, &m_renderC);
                b = Vector3D::DotProductVectors(&m_renderC, &m_renderA);
                c = Vector3D::DotProductVectors(&m_renderD, &m_renderA);
                d = m_renderC.GetMagnitude2();
                e = m_renderA.GetMagnitude2();

                denom = d * e - b * b;
                if (fabs(denom) <= TOLLERANCE)
                {
                    continue;
                }

                numer = a * b - c * d;

                rate = numer / denom;

                if (rate < 0.0f || rate > 1.0f)
                {
                    continue;
                }

                DrawLinePoint3D(pLine, x + y * m_pCamera->m_screenWidth, rate,
                                pLine->m_ppListVertexesClipped[firstIndex]->m_pos.m_z + rate * m_renderA.m_z, firstIndex, secondIndex);
            }
        }
        error -= deltaY;
        if (error < 0)
        {
            y += stepY;
            error += deltaX;
        }
    }
}

void Engine3D::DrawLinePoint3D(Line3D *pLine, const int offset, const float rate, const float z, const int firstIndex, const int secondIndex)
{
    if (!m_pCamera->SetValueZBuffer(offset, z, true))
    {
        return;
    }

    Color::InterpColors(&m_colorTemp, &pLine->m_ppListVertexesClipped[firstIndex]->m_color, &pLine->m_ppListVertexesClipped[secondIndex]->m_color, rate);

    m_colorTemp.CalcRGBA();

    if (m_colorTemp.m_alpha == 255)
    {
        m_pCamera->m_pScreenBuffer[offset] = m_colorTemp.m_rgba;
    }
    else
    {
        Color oldColor;

        oldColor.m_rgba = m_pCamera->m_pScreenBuffer[offset];
        oldColor.CalcRedGreenBlueAlpha();
        oldColor.Blend(&m_colorTemp);

        m_pCamera->m_pScreenBuffer[offset] = oldColor.m_rgba;
    }
}

bool Engine3D::DrawTriangle3D(Triangle3D *pTriangle)
{
    bool        ok = false;
    int         i;
    int         j;
    int         l;
    int         x;
    int         y;
    int         x0;
    int         y0;
    int         x1;
    int         y1;
    int         x2;
    int         y2;
    int         xx1;
    int         xx2;
    int         firstIndex;
    int         secondIndex;
    int         thirdIndex;
    int         offset1;
    int         offset2;
    int         deltaX[pTriangle->m_numVertexes];
    int         deltaY[pTriangle->m_numVertexes];
    float       a;
    float       a2;
    float       b;
    float       b2;
    float       c;
    float       c2;
    float       startX;
    float       endX;
    float       xf;
    float       yf;
    float       ratioColorLightOther;
    float       d[pTriangle->m_numVertexes];
    Vector2D    vectorA;
    Vector2D    vectorB;
    Vector2D    vectorC;
    Vector2D    vectorD;
    ExtVector3D *pVertexClipped;
    ExtVector3D *pOldVertexClipped = NULL;

    if (m_shading == SHADING_TYPE_FLAT)
    {
        if (m_pLightOther)
        {
            pTriangle->m_plane.Set(&((ExtVector3D *)m_listVertexesOriginal.GetElement(pTriangle->m_pListNumVertexes[0]).p)->m_pos,
                    &((ExtVector3D *)m_listVertexesOriginal.GetElement(pTriangle->m_pListNumVertexes[1]).p)->m_pos,
                    &((ExtVector3D *)m_listVertexesOriginal.GetElement(pTriangle->m_pListNumVertexes[2]).p)->m_pos);

            if (m_pLightOther->m_type == Light::TYPE_DIRECTIONAL)
            {
                ratioColorLightOther = Vector3D::DotProductVectors(&m_pLightOther->m_vectorRot, &pTriangle->m_plane.m_normal);
            }
            else if (m_pLightOther->m_type == Light::TYPE_POINT || m_pLightOther->m_type == Light::TYPE_SPOT)
            {
                Vector3D::CopyVector(&m_vectorTemp, &((ExtVector3D *)m_listVertexesOriginal.GetElement(pTriangle->m_pListNumVertexes[0]).p)->m_pos);
                Vector3D::AddVectors(&m_vectorTemp, &((ExtVector3D *)m_listVertexesOriginal.GetElement(pTriangle->m_pListNumVertexes[1]).p)->m_pos, &m_vectorTemp);
                Vector3D::AddVectors(&m_vectorTemp, &((ExtVector3D *)m_listVertexesOriginal.GetElement(pTriangle->m_pListNumVertexes[2]).p)->m_pos, &m_vectorTemp);

                Vector3D::MultiplyVectorScalar(&m_vectorTemp, &m_vectorTemp, 1.0f / 3.0f);

                Vector3D::SubVectors(&m_vectorTemp, &m_pLightOther->m_pos, &m_vectorTemp);
                Vector3D::NormalizeVector(&m_vectorTemp, &m_vectorTemp);

                ratioColorLightOther = Vector3D::DotProductVectors(&m_vectorTemp, &pTriangle->m_plane.m_normal);

                if (m_pLightOther->m_type == Light::TYPE_SPOT)
                {
                    if (ratioColorLightOther > 0.0f)
                    {
                        ratioColorLightOther *= Vector3D::DotProductVectors(&m_pLightOther->m_vectorRot, &pTriangle->m_plane.m_normal);
                    }
                    else
                    {
                        ratioColorLightOther = 0.0f;
                    }
                }
            }
            else
            {
                ratioColorLightOther = 0.0f;
            }
            if (ratioColorLightOther < 0.0f)
            {
                ratioColorLightOther = 0.0f;
            }
        }
        else
        {
            ratioColorLightOther = 0.0f;
        }

        for (i = 0; i < pTriangle->m_numVertexesClipped - 1; i ++)
        {
            pVertexClipped = pTriangle->m_ppListVertexesClipped[i];
            if (pVertexClipped == pOldVertexClipped)
            {
                continue;
            }
            else
            {
                pOldVertexClipped = pVertexClipped;
            }

            if (m_pLightOther)
            {
                if (m_pLightAmbient)
                {
                    Color::CopyColor(&m_colorTemp, &m_pLightAmbient->m_color);
                }
                else
                {
                    m_colorTemp.Set(0, 0, 0, 255);
                }

                Color::InterpColors(&m_colorTemp2, &m_colorTemp, &m_pLightOther->m_color, ratioColorLightOther);
                Color::MultiplyColors(&pVertexClipped->m_colorProcessed, &pVertexClipped->m_color, &m_colorTemp2);
            }
            else if (m_pLightAmbient)
            {
                Color::MultiplyColors(&pVertexClipped->m_colorProcessed, &pVertexClipped->m_color, &m_pLightAmbient->m_color);
            }
            else
            {
                Color::CopyColor(&pVertexClipped->m_colorProcessed, &pVertexClipped->m_color);
            }
        }
    }

    for (i = 0; i <= pTriangle->m_numVertexesClipped - pTriangle->m_numVertexes; i += 2)
    {
        firstIndex = i;
        secondIndex = firstIndex + 1;
        thirdIndex = secondIndex + 1;

        x0 = pTriangle->m_ppListVertexesClipped[firstIndex]->m_xOnScreen;
        y0 = pTriangle->m_ppListVertexesClipped[firstIndex]->m_yOnScreen;
        x1 = pTriangle->m_ppListVertexesClipped[secondIndex]->m_xOnScreen;
        y1 = pTriangle->m_ppListVertexesClipped[secondIndex]->m_yOnScreen;
        x2 = pTriangle->m_ppListVertexesClipped[thirdIndex]->m_xOnScreen;
        y2 = pTriangle->m_ppListVertexesClipped[thirdIndex]->m_yOnScreen;

        if ((x0 < 0 || x0 >= m_pCamera->m_screenWidth || y0 < 0 || y0 >= m_pCamera->m_screenHeight) &&
                (x1 < 0 || x1 >= m_pCamera->m_screenWidth || y1 < 0 || y1 >= m_pCamera->m_screenHeight) &&
                (x2 < 0 || x2 >= m_pCamera->m_screenWidth || y2 < 0 || y2 >= m_pCamera->m_screenHeight))
        {
            if ((x0 < 0 && x1 < 0 && x2 < 0) || (y0 < 0 && y1 < 0 && y2 < 0) ||
                    (x0 >= m_pCamera->m_screenWidth && x1 >= m_pCamera->m_screenWidth && x2 >= m_pCamera->m_screenWidth) ||
                    (y0 >= m_pCamera->m_screenHeight && y1 >= m_pCamera->m_screenHeight && y2 >= m_pCamera->m_screenHeight))
            {

                continue;
            }

            vectorA.Set(x0, y0);
            vectorB.Set(x1, y1);
            vectorC.Set(x2, y2);
            vectorD.Set(0.0f, 0.0f);

            if (Vector2D::PointInTriangle(&vectorD, &vectorA, &vectorB, &vectorC))
            {
                goto _exit2;
            }

            vectorD.Set(m_pCamera->m_screenWidth - 1.0f, 0.0f);

            if (Vector2D::PointInTriangle(&vectorD, &vectorA, &vectorB, &vectorC))
            {
                goto _exit2;
            }

            vectorD.Set(m_pCamera->m_screenWidth - 1.0f, m_pCamera->m_screenHeight - 1.0f);

            if (Vector2D::PointInTriangle(&vectorD, &vectorA, &vectorB, &vectorC))
            {
                goto _exit2;
            }

            vectorD.Set(0.0f, m_pCamera->m_screenHeight - 1.0f);

            if (Vector2D::PointInTriangle(&vectorD, &vectorA, &vectorB, &vectorC))
            {
                goto _exit2;
            }

            vectorC.Set(x0, y0);
            vectorD.Set(x1, y1);

            vectorA.Set(0.0f, 0.0f);
            vectorB.Set(m_pCamera->m_screenWidth - 1.0f, 0.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            vectorA.Set(m_pCamera->m_screenWidth - 1.0f, 0.0f);
            vectorB.Set(m_pCamera->m_screenWidth - 1.0f, m_pCamera->m_screenHeight - 1.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            vectorA.Set(m_pCamera->m_screenWidth - 1.0f, m_pCamera->m_screenHeight - 1.0f);
            vectorB.Set(0, m_pCamera->m_screenHeight - 1.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            vectorA.Set(0.0f, m_pCamera->m_screenHeight - 1.0f);
            vectorB.Set(0.0f, 0.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            vectorC.Set(x1, y1);
            vectorD.Set(x2, y2);

            vectorA.Set(0.0f, 0.0f);
            vectorB.Set(m_pCamera->m_screenWidth - 1.0f, 0.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            vectorA.Set(m_pCamera->m_screenWidth - 1.0f, 0.0f);
            vectorB.Set(m_pCamera->m_screenWidth - 1.0f, m_pCamera->m_screenHeight - 1.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            vectorA.Set(m_pCamera->m_screenWidth - 1.0f, m_pCamera->m_screenHeight - 1.0f);
            vectorB.Set(0, m_pCamera->m_screenHeight - 1.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            vectorA.Set(0.0f, m_pCamera->m_screenHeight - 1.0f);
            vectorB.Set(0.0f, 0.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            vectorC.Set(x2, y2);
            vectorD.Set(x0, y0);

            vectorA.Set(0.0f, 0.0f);
            vectorB.Set(m_pCamera->m_screenWidth - 1.0f, 0.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            vectorA.Set(m_pCamera->m_screenWidth - 1.0f, 0.0f);
            vectorB.Set(m_pCamera->m_screenWidth - 1.0f, m_pCamera->m_screenHeight - 1.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            vectorA.Set(m_pCamera->m_screenWidth - 1.0f, m_pCamera->m_screenHeight - 1.0f);
            vectorB.Set(0, m_pCamera->m_screenHeight - 1.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            vectorA.Set(0.0f, m_pCamera->m_screenHeight - 1.0f);
            vectorB.Set(0.0f, 0.0f);
            if (Vector2D::IntersectVectors(&vectorA, &vectorB, &vectorC, &vectorD))
            {
                goto _exit2;
            }

            continue;

_exit2:
            ;
        }

        ok = true;

        Vector3D::CopyVector(&m_renderOldP, &pTriangle->m_ppListVertexesClipped[firstIndex]->m_pos);

        Vector3D::SubVectors(&m_renderOldM, &pTriangle->m_ppListVertexesClipped[secondIndex]->m_pos, &m_renderOldP);

        Vector3D::SubVectors(&m_renderOldN, &pTriangle->m_ppListVertexesClipped[thirdIndex]->m_pos, &m_renderOldP);

        m_renderNewP.Set(m_renderOldP.m_x * m_pCamera->m_screenDistX, m_renderOldP.m_y * m_pCamera->m_screenDistY, m_renderOldP.m_z);

        m_renderNewM.Set(m_renderOldM.m_x * m_pCamera->m_screenDistX, m_renderOldM.m_y * m_pCamera->m_screenDistY, m_renderOldM.m_z);

        m_renderNewN.Set(m_renderOldN.m_x * m_pCamera->m_screenDistX, m_renderOldN.m_y * m_pCamera->m_screenDistY, m_renderOldN.m_z);

        Vector3D::CrossProductVectors(&m_renderA, &m_renderNewN, &m_renderNewP);

        Vector3D::CrossProductVectors(&m_renderB, &m_renderNewP, &m_renderNewM);

        Vector3D::CrossProductVectors(&m_renderC, &m_renderNewM, &m_renderNewN);

        if (y1 < y0)
        {
            l = y1;
            y1 = y0;
            y0 = l;

            l = x1;
            x1 = x0;
            x0 = l;
        }
        if (y2 < y0)
        {
            l = y2;
            y2 = y0;
            y0 = l;

            l = x2;
            x2 = x0;
            x0 = l;
        }
        if (y2 < y1)
        {
            l = y2;
            y2 = y1;
            y1 = l;

            l = x2;
            x2 = x1;
            x1 = l;
        }

        deltaX[0] = x1 - x0;
        deltaY[0] = y1 - y0;
        deltaX[1] = x2 - x1;
        deltaY[1] = y2 - y1;
        deltaX[2] = x2 - x0;
        deltaY[2] = y2 - y0;

        for (j = 0; j < pTriangle->m_numVertexes; j ++)
        {
            if (deltaY[j] != 0)
            {
                d[j] = (float)deltaX[j] / (float)deltaY[j];
            }
            else
            {
                d[j] = 0.0f;
            }
        }

        startX = x0;
        endX = x0;
        offset1 = y0 * m_pCamera->m_screenWidth;
        yf = y0 - m_pCamera->m_screenCenterY + 0.5f;
        a = m_renderA.m_z + yf * m_renderA.m_y;
        b = m_renderB.m_z + yf * m_renderB.m_y;
        c = m_renderC.m_z + yf * m_renderC.m_y;
        for (y = y0; y < y1; y ++, startX += d[0], endX += d[2], offset1 += m_pCamera->m_screenWidth, yf += 1.0f,
             a += m_renderA.m_y, b += m_renderB.m_y, c += m_renderC.m_y)
        {
            if (y < 0)
            {
                continue;
            }
            else if (y >= m_pCamera->m_screenHeight)
            {
                break;
            }

            if (startX < endX)
            {
                xx1 = (int)startX;
                xx2 = (int)endX;
            }
            else
            {
                xx1 = (int)endX;
                xx2 = (int)startX;
            }
            offset2 = offset1 + xx1;
            xf = xx1 - m_pCamera->m_screenCenterX + 0.5f;
            a2 = a + xf * m_renderA.m_x;
            b2 = b + xf * m_renderB.m_x;
            c2 = c + xf * m_renderC.m_x;
            for (x = xx1; x <= xx2; x ++, offset2 ++, xf += 1.0f,
                 a2 += m_renderA.m_x, b2 += m_renderB.m_x, c2 += m_renderC.m_x)
            {
                if (x < 0 || c2 == 0.0f)
                {
                    continue;
                }
                else if (x >= m_pCamera->m_screenWidth)
                {
                    break;
                }

                DrawTrianglePoint3D(pTriangle, offset2, firstIndex, secondIndex, thirdIndex, a2, b2, c2);
            }
        }

        startX = x1;
        endX = x0 + (y1 - y0) * d[2];
        offset1 = y1 * m_pCamera->m_screenWidth;
        yf = y1 - m_pCamera->m_screenCenterY + 0.5f;
        a = m_renderA.m_z + yf * m_renderA.m_y;
        b = m_renderB.m_z + yf * m_renderB.m_y;
        c = m_renderC.m_z + yf * m_renderC.m_y;
        for (y = y1; y <= y2; y ++, startX += d[1], endX += d[2], offset1 += m_pCamera->m_screenWidth, yf += 1.0f,
             a += m_renderA.m_y, b += m_renderB.m_y, c += m_renderC.m_y)
        {
            if (y < 0)
            {
                continue;
            }
            else if (y >= m_pCamera->m_screenHeight)
            {
                break;
            }

            if (startX < endX)
            {
                xx1 = (int)startX;
                xx2 = (int)endX;
            }
            else
            {
                xx1 = (int)endX;
                xx2 = (int)startX;
            }
            offset2 = offset1 + xx1;
            xf = xx1 - m_pCamera->m_screenCenterX + 0.5f;
            a2 = a + xf * m_renderA.m_x;
            b2 = b + xf * m_renderB.m_x;
            c2 = c + xf * m_renderC.m_x;
            for (x = xx1; x <= xx2; x ++, offset2 ++, xf += 1.0f,
                 a2 += m_renderA.m_x, b2 += m_renderB.m_x, c2 += m_renderC.m_x)
            {
                if (x < 0 || c2 == 0.0f)
                {
                    continue;
                }
                else if (x >= m_pCamera->m_screenWidth)
                {
                    break;
                }

                DrawTrianglePoint3D(pTriangle, offset2, firstIndex, secondIndex, thirdIndex, a2, b2, c2);
            }
        }
    }

    return ok;
}

void Engine3D::DrawTrianglePoint3D(Triangle3D *pTriangle, const int offset, const int firstIndex, const int secondIndex, const int thirdIndex,
                                   const float a, const float b, const float c)
{
    int     xx = 0;
    int     yy = 0;
    int     *pTextureBuffer;
    float   u;
    float   v;
    float   newU;
    float   newV;
    float   d;

    u = a / c;
    v = b / c;

    if (u < 0.0f)
    {
        u = 0.0f;
    }
    if (v < 0.0f)
    {
        v = 0.0f;
    }

    if (u > 1.0f)
    {
        u = 1.0f;
    }
    if (v > 1.0f)
    {
        v = 1.0f;
    }

    /*m_pointTemp.set(oldP.m_x + u * oldM.m_x + v * oldN.m_x,
            oldP.m_y + u * oldM.m_y + v * oldN.m_y,
            oldP.m_z + u * oldM.m_z + v * oldN.m_z);*/

    if (!m_pCamera->SetValueZBuffer(offset, m_renderOldP.m_z + u * m_renderOldM.m_z + v * m_renderOldN.m_z/*m_pointTemp.m_z*/, true))
    {
        return;
    }

    d = 1.0f - u - v;
    if (d < 0.0f)
    {
        d /= 2.0f;
        u += d;
        v += d;
        d = 0.0f;
    }

    if (m_shading == SHADING_TYPE_FLAT)
    {
        m_colorTemp.m_red = (int)(u * pTriangle->m_ppListVertexesClipped[secondIndex]->m_colorProcessed.m_red +
                                  v * pTriangle->m_ppListVertexesClipped[thirdIndex]->m_colorProcessed.m_red +
                                  d * pTriangle->m_ppListVertexesClipped[firstIndex]->m_colorProcessed.m_red);
        m_colorTemp.m_green = (int)(u * pTriangle->m_ppListVertexesClipped[secondIndex]->m_colorProcessed.m_green +
                                    v * pTriangle->m_ppListVertexesClipped[thirdIndex]->m_colorProcessed.m_green +
                                    d * pTriangle->m_ppListVertexesClipped[firstIndex]->m_colorProcessed.m_green);
        m_colorTemp.m_blue = (int)(u * pTriangle->m_ppListVertexesClipped[secondIndex]->m_colorProcessed.m_blue +
                                   v * pTriangle->m_ppListVertexesClipped[thirdIndex]->m_colorProcessed.m_blue +
                                   d * pTriangle->m_ppListVertexesClipped[firstIndex]->m_colorProcessed.m_blue);
        m_colorTemp.m_alpha = (int)(u * pTriangle->m_ppListVertexesClipped[secondIndex]->m_colorProcessed.m_alpha +
                                   v * pTriangle->m_ppListVertexesClipped[thirdIndex]->m_colorProcessed.m_alpha +
                                   d * pTriangle->m_ppListVertexesClipped[firstIndex]->m_colorProcessed.m_alpha);
    }
    else
    {
        m_colorTemp.m_red = (int)(u * pTriangle->m_ppListVertexesClipped[secondIndex]->m_color.m_red +
                                  v * pTriangle->m_ppListVertexesClipped[thirdIndex]->m_color.m_red +
                                  d * pTriangle->m_ppListVertexesClipped[firstIndex]->m_color.m_red);
        m_colorTemp.m_green = (int)(u * pTriangle->m_ppListVertexesClipped[secondIndex]->m_color.m_green +
                                    v * pTriangle->m_ppListVertexesClipped[thirdIndex]->m_color.m_green +
                                    d * pTriangle->m_ppListVertexesClipped[firstIndex]->m_color.m_green);
        m_colorTemp.m_blue = (int)(u * pTriangle->m_ppListVertexesClipped[secondIndex]->m_color.m_blue +
                                   v * pTriangle->m_ppListVertexesClipped[thirdIndex]->m_color.m_blue +
                                   d * pTriangle->m_ppListVertexesClipped[firstIndex]->m_color.m_blue);
        m_colorTemp.m_alpha = (int)(u * pTriangle->m_ppListVertexesClipped[secondIndex]->m_color.m_alpha +
                                   v * pTriangle->m_ppListVertexesClipped[thirdIndex]->m_color.m_alpha +
                                   d * pTriangle->m_ppListVertexesClipped[firstIndex]->m_color.m_alpha);
    }

    if (pTriangle->m_textureIndex >= TexturesManager::TEXTURE_FIRST && pTriangle->m_textureIndex < TexturesManager::NUM_TEXTURES)
    {
        newU  = u * pTriangle->m_ppListVertexesClipped[secondIndex]->m_uv.m_x +
                v * pTriangle->m_ppListVertexesClipped[thirdIndex]->m_uv.m_x +
                d * pTriangle->m_ppListVertexesClipped[firstIndex]->m_uv.m_x;
        newV  = u * pTriangle->m_ppListVertexesClipped[secondIndex]->m_uv.m_y +
                v * pTriangle->m_ppListVertexesClipped[thirdIndex]->m_uv.m_y +
                d * pTriangle->m_ppListVertexesClipped[firstIndex]->m_uv.m_y;

        xx = (int)(newU * (float)(g_pTexturesManager->GetTextureWidth((TexturesManager::Textures)pTriangle->m_textureIndex) - 1));
        yy = (int)(newV * (float)(g_pTexturesManager->GetTextureHeight((TexturesManager::Textures)pTriangle->m_textureIndex) - 1));

        pTextureBuffer = g_pTexturesManager->GetTextureBuffer((TexturesManager::Textures)pTriangle->m_textureIndex);

        m_colorTemp2.m_rgba = pTextureBuffer[xx + yy * g_pTexturesManager->GetTextureWidth((TexturesManager::Textures)pTriangle->m_textureIndex)];
        m_colorTemp2.CalcRedGreenBlueAlpha();
        Color::MultiplyColors(&m_colorTemp, &m_colorTemp, &m_colorTemp2);
    }

    m_colorTemp.CalcRGBA();

    if (m_colorTemp.m_alpha == 255)
    {
        m_pCamera->m_pScreenBuffer[offset] = m_colorTemp.m_rgba;
    }
    else
    {
        Color oldColor;

        oldColor.m_rgba = m_pCamera->m_pScreenBuffer[offset];
        oldColor.CalcRedGreenBlueAlpha();
        oldColor.Blend(&m_colorTemp);

        m_pCamera->m_pScreenBuffer[offset] = oldColor.m_rgba;
    }
}

