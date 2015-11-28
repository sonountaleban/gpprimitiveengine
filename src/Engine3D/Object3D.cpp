#include "Engine3D/Object3D.h"
#include "Engine3D/ExtVector3D.h"
#include "Engine3D/Triangle3D.h"
#include "TexturesManager.h"

Object3D::Object3D()
{
    m_pMeshHead = NULL;
    m_pMeshTail = NULL;

    m_pMaterialsHead = NULL;
    m_pMaterialsTail = NULL;

    m_scale.Set(1.0f, 1.0f, 1.0f);

    UpdateMatrix();
}

Object3D::~Object3D()
{
    DestroyMeshAndMaterials();

    DestroyVertexesAndTriangles();
}

bool Object3D::Load3DS(const char *pFileName)
{
    char        fileName[256];
    FILE        *pFile;
    Material    *pMaterialCurr;
    Texture     *pTextureCurr;

    DEBUGLOG("Load object 3DS %s: ", pFileName);

    sprintf(fileName, "resources/%s.3ds", pFileName);

    pFile = fopen(fileName, "rb");
    if (pFile)
    {
        if (fseek(pFile, 0, SEEK_SET))
        {
            fclose(pFile);

            DEBUGLOG("Error!\n\n");

            return false;
        }

        if (ReadPrimaryChunk(pFile))
        {
            pMaterialCurr = m_pMaterialsHead;
            if (pMaterialCurr)
            {
                do
                {
                    pTextureCurr = pMaterialCurr->pTexturesHead;
                    if (pTextureCurr)
                    {
                        do
                        {
                            //pTextures_manager->add(pTexture_curr->name);

                            pTextureCurr = pTextureCurr->pTextureNext;
                        } while (pTextureCurr);
                    }

                    pMaterialCurr = pMaterialCurr->pMaterialNext;
                } while (pMaterialCurr);
            }

            DEBUGLOG("OK.\n\n");

            PrintMeshInfo();
            PrintMaterialsInfo();

            DEBUGLOG("\n");

            InitVertexesAndTriangles();

            return true;
        }
    }

    DEBUGLOG("Error!\n\n");

    return false;
}

void Object3D::PrintMeshInfo()
{
    int         a;
    int         i = 0;
    int         j;
    Mesh        *pMeshCurr;
    FacesMat    *pFacesMatCurr;

    if (!m_pMeshHead)
    {
        DEBUGLOG("Object3D::PrintMeshInfo -> No mesh!\n");

        return;
    }

    pMeshCurr = m_pMeshHead;
    do
    {
        DEBUGLOG("Mesh %d: %s\n", i, pMeshCurr->name);

        DEBUGLOG("  Num. of vertices: %d\n", pMeshCurr->numVertices);
        for (a = 0; a < pMeshCurr->numVertices; a ++)
        {
            DEBUGLOG("    %d -> x: %f; y: %f; z: %f\n", a, pMeshCurr->pVerticesList[a].x, pMeshCurr->pVerticesList[a].y, pMeshCurr->pVerticesList[a].z);
        }

        DEBUGLOG("  Num. of faces: %d\n", pMeshCurr->numFaces);
        for (a = 0; a < pMeshCurr->numFaces; a ++)
        {
            DEBUGLOG("    %d -> indexes: %d - %d - %d; order: %d\n", a, pMeshCurr->pFacesList[a * 4], pMeshCurr->pFacesList[a * 4 + 1],
                pMeshCurr->pFacesList[a * 4 + 2], pMeshCurr->pFacesList[a * 4 + 3]);
        }

        DEBUGLOG("  Num. of mapping coords: %d\n", pMeshCurr->numMappingCoords);
        for (a = 0; a < pMeshCurr->numMappingCoords; a ++)
        {
            DEBUGLOG("    %d -> x: %f; y: %f\n", a, pMeshCurr->pMappingCoords[a].x, pMeshCurr->pMappingCoords[a].y);
        }

        DEBUGLOG("  Local transformation matrix:\n");
        for (a = 0; a < 4; a ++)
        {
            DEBUGLOG("    %f %f %f %f\n", pMeshCurr->localTransMatrix[a][0], pMeshCurr->localTransMatrix[a][1],
                pMeshCurr->localTransMatrix[a][2], pMeshCurr->localTransMatrix[a][3]);
        }

        pFacesMatCurr = pMeshCurr->pFacesMatHead;
        if (pFacesMatCurr)
        {
            j = 0;
            do
            {
                DEBUGLOG("  Material %d: %s\n", j, pFacesMatCurr->name);
                DEBUGLOG("    Num. of entries: %d\n", pFacesMatCurr->numEntries);
                for (a = 0; a < pFacesMatCurr->numEntries; a ++)
                {
                    DEBUGLOG("      %d -> face: %d\n", a, pFacesMatCurr->pEntries[a]);
                }

                j ++;

                pFacesMatCurr = pFacesMatCurr->pFacesMatNext;
            } while (pFacesMatCurr);
        }

        i ++;

        pMeshCurr = pMeshCurr->pMeshNext;
    } while (pMeshCurr);
}

void Object3D::PrintMaterialsInfo()
{
    int         i = 0;
    int         j;
    Material    *pMaterialCurr;
    Texture     *pTextureCurr;

    if (!m_pMaterialsHead)
    {
        DEBUGLOG("Object3D::PrintMaterialsInfo -> No materials!\n");

        return;
    }

    pMaterialCurr = m_pMaterialsHead;
    do
    {
        DEBUGLOG("Material %d: %s\n", i, pMaterialCurr->name);

        DEBUGLOG("  Ambient color: 0x%x\n", pMaterialCurr->colorAmbient);
        DEBUGLOG("  Diffuse color: 0x%x\n", pMaterialCurr->colorDiffuse);
        DEBUGLOG("  Specular color: 0x%x\n", pMaterialCurr->colorSpecular);

        pTextureCurr = pMaterialCurr->pTexturesHead;
        if (pTextureCurr)
        {
            j = 0;
            do
            {
                DEBUGLOG("     Texture %d: %s\n", j, pTextureCurr->name);
                DEBUGLOG("       u: %f\n", pTextureCurr->u);
                DEBUGLOG("       v: %f\n", pTextureCurr->v);
                DEBUGLOG("       u_off: %f\n", pTextureCurr->uOff);
                DEBUGLOG("       v_off: %f\n", pTextureCurr->vOff);
                DEBUGLOG("       rot: %f\n", pTextureCurr->rot);

                j ++;

                pTextureCurr = pTextureCurr->pTextureNext;
            } while (pTextureCurr);
        }

        i ++;

        pMaterialCurr = pMaterialCurr->pMaterialNext;
    } while (pMaterialCurr);
}

void Object3D::UpdateMatrix()
{
    m_matrix.SetIdentity();
    m_matrix.Scale(m_scale.m_x, m_scale.m_y, m_scale.m_z);
    m_matrix.Rotate(m_angle.m_x, m_angle.m_y, m_angle.m_z);
}

void Object3D::Render()
{
    int         a;
    int         vertexIndex = g_pEngine3D->GetNumVertexes3D();
    Element     e;
    Triangle3D  *pTriangle;

    for (a = 0; a < m_listVertexes.GetNumElements(); a ++)
    {
        e = m_listVertexes.GetElement(a);
        if (e.p)
        {
            g_pEngine3D->AddVertex3D((ExtVector3D *)e.p);
        }
    }

    for (a = 0; a < m_listTriangles3D.GetNumElements(); a ++)
    {
        e = m_listTriangles3D.GetElement(a);
        if (e.p)
        {
            pTriangle = (Triangle3D *)e.p;
            pTriangle->ResetListNumVertexes();
            pTriangle->AddOffsetToListNumVertexes(vertexIndex);

            g_pEngine3D->AddPrimitive3D(pTriangle);
        }
    }
}

void Object3D::InitVertexesAndTriangles()
{
    int         a;
    int         index;
    int         face;
    int         vertexIndex = m_listVertexes.GetNumElements();
    float       u;
    float       v;
    Color       color;
    Mesh        *pMeshCurr;
    FacesMat    *pFacesMatCurr;
    Material    *pMaterialCurr;
    Texture     *pTextureCurr;
    Element     e;
    Vector3D    vector1;
    Vector3D    vector2;
    int         textureIndex;

    if (!m_pMeshHead || !m_pMaterialsHead)
    {
        return;
    }

    pMeshCurr = m_pMeshHead;
    do
    {
        pFacesMatCurr = pMeshCurr->pFacesMatHead;
        if (pFacesMatCurr)
        {
            do
            {
                pMaterialCurr = m_pMaterialsHead;
                do
                {
                    if (_strcasecmp(pFacesMatCurr->name, pMaterialCurr->name))
                    {
                        pTextureCurr = pMaterialCurr->pTexturesHead;
                        if (pTextureCurr)
                        {
                            textureIndex = g_pTexturesManager->GetTextureIndex(pTextureCurr->name);
                            //glBindTexture(GL_TEXTURE_2D, pTextures_manager->getGenerated(pTexture_curr->name, filter));
                        }
                        else
                        {
                            textureIndex = TexturesManager::TEXTURE_NONE;
                        }

                        //glBegin(GL_TRIANGLES);

                        for (a = 0; a < pFacesMatCurr->numEntries; a ++)
                        {
                            face = pFacesMatCurr->pEntries[a];
                            if (face < pMeshCurr->numFaces)
                            {
                                index = pMeshCurr->pFacesList[face * 4];

                                /*if (shading == SHADING_SMOOTH)
                                    glNormal3f(pMesh_curr->pNormals_vertices_list[index].x,
                                        pMesh_curr->pNormals_vertices_list[index].y,
                                        pMesh_curr->pNormals_vertices_list[index].z);
                                else
                                    glNormal3f(pMesh_curr->pNormals_faces_list[face].x,
                                        pMesh_curr->pNormals_faces_list[face].y,
                                        pMesh_curr->pNormals_faces_list[face].z);*/

                                vector1.Set(pMeshCurr->pVerticesList[index].x, pMeshCurr->pVerticesList[index].y, pMeshCurr->pVerticesList[index].z);

                                Vector3D::MultiplyVectorMatrix(&vector2, &vector1, &m_matrix);

                                Vector3D::AddVectors(&vector2, &vector2, &m_pos);

                                color.Set(pMaterialCurr->colorDiffuse & 0xff, (pMaterialCurr->colorDiffuse & 0xff00) >> 8,
                                          (pMaterialCurr->colorDiffuse & 0xff0000) >> 16, (pMaterialCurr->colorDiffuse & 0xff000000) >> 24);

                                if (pTextureCurr)
                                {
                                    u = pMeshCurr->pMappingCoords[index].x;
                                    v = pMeshCurr->pMappingCoords[index].y;

                                    e.p = new ExtVector3D(vector2.m_x, vector2.m_y, vector2.m_z, &color, u, v);
                                }
                                else
                                {
                                    e.p = new ExtVector3D(vector2.m_x, vector2.m_y, vector2.m_z, &color);
                                }
                                m_listVertexes.AddElement(e);

                                if (pMeshCurr->pFacesList[face * 4 + 1] == 0)
                                {
                                    index = pMeshCurr->pFacesList[face * 4 + 1];
                                }
                                else
                                {
                                    index = pMeshCurr->pFacesList[face * 4 + 2];
                                }

                                /*if (shading == SHADING_SMOOTH)
                                    glNormal3f(pMesh_curr->pNormals_vertices_list[index].x,
                                        pMesh_curr->pNormals_vertices_list[index].y,
                                        pMesh_curr->pNormals_vertices_list[index].z);
                                else
                                    glNormal3f(pMesh_curr->pNormals_faces_list[face].x,
                                        pMesh_curr->pNormals_faces_list[face].y,
                                        pMesh_curr->pNormals_faces_list[face].z);*/

                                vector1.Set(pMeshCurr->pVerticesList[index].x, pMeshCurr->pVerticesList[index].y, pMeshCurr->pVerticesList[index].z);

                                Vector3D::MultiplyVectorMatrix(&vector2, &vector1, &m_matrix);

                                Vector3D::AddVectors(&vector2, &vector2, &m_pos);

                                color.Set(pMaterialCurr->colorDiffuse & 0xff, (pMaterialCurr->colorDiffuse & 0xff00) >> 8,
                                          (pMaterialCurr->colorDiffuse & 0xff0000) >> 16, (pMaterialCurr->colorDiffuse & 0xff000000) >> 24);

                                if (pTextureCurr)
                                {
                                    u = pMeshCurr->pMappingCoords[index].x;
                                    v = pMeshCurr->pMappingCoords[index].y;

                                    e.p = new ExtVector3D(vector2.m_x, vector2.m_y, vector2.m_z, &color, u, v);
                                }
                                else
                                {
                                    e.p = new ExtVector3D(vector2.m_x, vector2.m_y, vector2.m_z, &color);
                                }
                                m_listVertexes.AddElement(e);

                                if (pMeshCurr->pFacesList[face * 4 + 1] == 0)
                                {
                                    index = pMeshCurr->pFacesList[face * 4 + 2];
                                }
                                else
                                {
                                    index = pMeshCurr->pFacesList[face * 4 + 1];
                                }

                                /*if (shading == SHADING_SMOOTH)
                                    glNormal3f(pMesh_curr->pNormals_vertices_list[index].x,
                                        pMesh_curr->pNormals_vertices_list[index].y,
                                        pMesh_curr->pNormals_vertices_list[index].z);
                                else
                                    glNormal3f(pMesh_curr->pNormals_faces_list[face].x,
                                        pMesh_curr->pNormals_faces_list[face].y,
                                        pMesh_curr->pNormals_faces_list[face].z);*/

                                vector1.Set(pMeshCurr->pVerticesList[index].x, pMeshCurr->pVerticesList[index].y, pMeshCurr->pVerticesList[index].z);

                                Vector3D::MultiplyVectorMatrix(&vector2, &vector1, &m_matrix);

                                Vector3D::AddVectors(&vector2, &vector2, &m_pos);

                                color.Set(pMaterialCurr->colorDiffuse & 0xff, (pMaterialCurr->colorDiffuse & 0xff00) >> 8,
                                          (pMaterialCurr->colorDiffuse & 0xff0000) >> 16, (pMaterialCurr->colorDiffuse & 0xff000000) >> 24);

                                if (pTextureCurr)
                                {
                                    u = pMeshCurr->pMappingCoords[index].x;
                                    v = pMeshCurr->pMappingCoords[index].y;

                                    e.p = new ExtVector3D(vector2.m_x, vector2.m_y, vector2.m_z, &color, u, v);
                                }
                                else
                                {
                                    e.p = new ExtVector3D(vector2.m_x, vector2.m_y, vector2.m_z, &color);
                                }
                                m_listVertexes.AddElement(e);

                                e.p = new Triangle3D(textureIndex, vertexIndex, vertexIndex + 1, vertexIndex + 2);
                                m_listTriangles3D.AddElement(e);

                                vertexIndex += 3;
                            }
                        }

                        //glEnd();

                        break;
                    }

                    pMaterialCurr = pMaterialCurr->pMaterialNext;
                } while (pMaterialCurr);

                pFacesMatCurr = pFacesMatCurr->pFacesMatNext;
            } while (pFacesMatCurr);
        }

        pMeshCurr = pMeshCurr->pMeshNext;
    } while (pMeshCurr);
}

void Object3D::DestroyVertexesAndTriangles()
{
    int     a;
    Element e;

    for (a = 0; a < m_listVertexes.GetNumElements(); a ++)
    {
        e = m_listVertexes.GetElement(a);
        if (e.p)
        {
            delete (ExtVector3D *)e.p;
        }
    }
    m_listVertexes.RemoveAllElements();

    for (a = 0; a < m_listTriangles3D.GetNumElements(); a ++)
    {
        e = m_listTriangles3D.GetElement(a);
        if (e.p)
        {
            delete (Triangle3D *)e.p;
        }
    }
    m_listTriangles3D.RemoveAllElements();
}

void Object3D::DestroyMeshAndMaterials()
{
    Mesh        *pMeshCurr;
    Mesh        *pMeshNext;
    FacesMat    *pFacesMatCurr;
    FacesMat    *pFacesMatNext;
    Material    *pMaterialCurr;
    Material    *pMaterialNext;
    Texture     *pTextureCurr;
    Texture     *pTextureNext;

    if (m_pMeshHead)
    {
        pMeshCurr = m_pMeshHead;
        do
        {
            pMeshNext = pMeshCurr->pMeshNext;

            if (pMeshCurr->pVerticesList)
            {
                delete[] pMeshCurr->pVerticesList;
            }

            if (pMeshCurr->pFacesList)
            {
                delete[] pMeshCurr->pFacesList;
            }

            if (pMeshCurr->pNormalsVerticesList)
            {
                delete[] pMeshCurr->pNormalsVerticesList;
            }

            if (pMeshCurr->pNormalsFacesList)
            {
                delete[] pMeshCurr->pNormalsFacesList;
            }

            if (pMeshCurr->pMappingCoords)
            {
                delete[] pMeshCurr->pMappingCoords;
            }

            if (pMeshCurr->pFacesMatHead)
            {
                pFacesMatCurr = pMeshCurr->pFacesMatHead;
                do
                {
                    pFacesMatNext = pFacesMatCurr->pFacesMatNext;

                    if (pFacesMatCurr->pEntries)
                    {
                        delete[] pFacesMatCurr->pEntries;
                    }

                    delete pFacesMatCurr;

                    pFacesMatCurr = pFacesMatNext;
                } while (pFacesMatCurr);
            }

            delete pMeshCurr;

            pMeshCurr = pMeshNext;
        } while (pMeshCurr);

        m_pMeshHead = NULL;
    }

    m_pMeshTail = NULL;

    if (m_pMaterialsHead)
    {
        pMaterialCurr = m_pMaterialsHead;
        do
        {
            pMaterialNext = pMaterialCurr->pMaterialNext;

            if (pMaterialCurr->pTexturesHead)
            {
                pTextureCurr = pMaterialCurr->pTexturesHead;
                do
                {
                    pTextureNext = pTextureCurr->pTextureNext;

                    delete pTextureCurr;

                    pTextureCurr = pTextureNext;
                } while (pTextureCurr);
            }

            delete pMaterialCurr;

            pMaterialCurr = pMaterialNext;
        } while (pMaterialCurr);

        m_pMaterialsHead = NULL;
    }

    m_pMaterialsTail = NULL;
}

bool Object3D::ReadPrimaryChunk(FILE *pFile)
{
    int version;

    if (ReadShort(pFile) == 0x4d4d)
    {
        if (fseek(pFile, 28L, SEEK_SET))
        {
            return false;
        }

        version = ReadChar(pFile);
        DEBUGLOG("version: %d\n", version);

        if (fseek(pFile, 16, SEEK_SET))
        {
            return false;
        }

        return Read3DS(pFile);
    }
    else
    {
        return false;
    }
}

bool Object3D::Read3DS(FILE *pFile)
{
    int     id;
    int     len;
    int     done = 0;
    long    pos;

    while (!done)
    {
        id = ReadShort(pFile);

        if (feof(pFile))
        {
            done = 1;

            break;
        }

        len = ReadInt(pFile);

        switch (id)
        {
            case 0xffff:
                done = 1;

                break;

            case 0x3d3d:
                ReadObject(pFile, len);

                break;

            default:
                pos = ftell(pFile) + (len - 6);
                if (fseek(pFile, pos, SEEK_SET))
                {
                    done = 1;
                }

                break;
        }

        if (feof(pFile))
        {
            done = 1;
        }
    }

    return true;
}

void Object3D::ReadObject(FILE *pFile, int len)
{
    char        ch;
    int         llen;
    int         done = 0;
    int         u;
    int         id;
    long        count = ftell(pFile) + (len - 6);
    long        pos;
    Mesh        *pMeshT;
    Mesh        *pMeshP;
    FacesMat    *pFacesMatCurr;
    FacesMat    *pFacesMatNext;

    DestroyMeshAndMaterials();

    DestroyVertexesAndTriangles();

    while (!done)
    {
        id = ReadShort(pFile);

        if (feof(pFile))
        {
            done = 1;

            break;
        }

        llen = ReadInt(pFile);

        switch (id)
        {
            case 0x4000:
                if (!m_pMeshHead)
                {
                    m_pMeshHead = new Mesh();

                    m_pMeshTail = m_pMeshHead;
                }
                else
                {
                    m_pMeshTail->pMeshNext = new Mesh();

                    m_pMeshTail = m_pMeshTail->pMeshNext;
                }

                memset(m_pMeshTail, 0, sizeof(Mesh));

                u = 0;
                while ((ch = ReadChar(pFile)))
                {
                    m_pMeshTail->name[u] = ch;
                    u ++;
                }
                m_pMeshTail->name[u] = '\0';

                ReadMesh(pFile, llen);

                if (m_pMeshTail->numVertices <= 0)
                {
                    DEBUGLOG("Object3D::ReadObject -> m_pMeshTail->numVertices <= 0!\n");
                }

                if (m_pMeshTail->numFaces <= 0)
                {
                    DEBUGLOG("Object3D::ReadObject -> m_pMeshTail->numFaces <= 0!\n");
                }

                if (m_pMeshTail->numVertices <= 0 || m_pMeshTail->numFaces <= 0)
                {
                    if (m_pMeshTail->pVerticesList)
                    {
                        delete[] m_pMeshTail->pVerticesList;
                    }

                    if (m_pMeshTail->pFacesList)
                    {
                        delete[] m_pMeshTail->pFacesList;
                    }

                    if (m_pMeshTail->pNormalsVerticesList)
                    {
                        delete[] m_pMeshTail->pNormalsVerticesList;
                    }

                    if (m_pMeshTail->pNormalsFacesList)
                    {
                        delete[] m_pMeshTail->pNormalsFacesList;
                    }

                    if (m_pMeshTail->pMappingCoords)
                    {
                        delete[] m_pMeshTail->pMappingCoords;
                    }

                    if (m_pMeshTail->pFacesMatHead)
                    {
                        pFacesMatCurr = m_pMeshTail->pFacesMatHead;
                        do
                        {
                            pFacesMatNext = pFacesMatCurr->pFacesMatNext;

                            if (pFacesMatCurr->pEntries)
                            {
                                delete[] pFacesMatCurr->pEntries;
                            }

                            delete pFacesMatCurr;

                            pFacesMatCurr = pFacesMatNext;
                        } while (pFacesMatCurr);
                    }

                    if (m_pMeshTail == m_pMeshHead)
                    {
                        delete m_pMeshTail;

                        m_pMeshTail = m_pMeshHead = NULL;
                    }
                    else
                    {
                        pMeshT = pMeshP = m_pMeshHead;
                        while (pMeshT)
                        {
                            if (pMeshT == m_pMeshTail)
                            {
                                break;
                            }
                            else
                            {
                                pMeshP = pMeshT;
                                pMeshT = pMeshT->pMeshNext;
                            }
                        }

                        delete m_pMeshTail;

                        m_pMeshTail = pMeshP;
                        m_pMeshTail->pMeshNext = NULL;
                    }
                }
                else
                {
                    CalcNormals();
                }

                break;

            case 0xafff:
                ReadMaterials(pFile, llen);

                break;

            default:
                DEBUGLOG("Object3D::ReadObject -> Unknown chunk! (%x)\n", id);

                pos = ftell(pFile);
                if ((pos - 6) >= count)
                {
                    fseek(pFile, -6, SEEK_CUR);

                    done = 1;

                    break;
                }
                pos += (llen - 6);
                if (fseek(pFile, pos, SEEK_SET))
                {
                    done = 1;
                }

                break;
        }
    }
}

void Object3D::ReadMesh(FILE *pFile, int len)
{
    char    ch;
    int     id;
    int     llen;
    int     done  = 0;
    int     i;
    int     j;
    long    pos;
    long    count = ftell(pFile) + (len - 6);

    while (!done)
    {
        id = ReadShort(pFile);

        if (feof(pFile))
        {
            done = 1;

            break;
        }

        llen = ReadInt(pFile);

        switch (id)
        {
            case 0x4100:
                if (!m_pMeshTail)
                {
                    break;
                }

                break;

            case 0x4110:
                if (!m_pMeshTail)
                {
                    break;
                }

                m_pMeshTail->numVertices = ReadShort(pFile);

                m_pMeshTail->pVerticesList = new FVector3D[m_pMeshTail->numVertices];

                m_pMeshTail->pNormalsVerticesList = new FVector3D[m_pMeshTail->numVertices];

                memset(m_pMeshTail->pNormalsVerticesList, 0, sizeof(FVector3D) * m_pMeshTail->numVertices);

                for (i = 0; i < m_pMeshTail->numVertices; i ++)
                {
                    fread(&(m_pMeshTail->pVerticesList[i].x), sizeof(float), 1, pFile);
                    fread(&(m_pMeshTail->pVerticesList[i].z), sizeof(float), 1, pFile);
                    fread(&(m_pMeshTail->pVerticesList[i].y), sizeof(float), 1, pFile);
                }

                break;

            case 0x4120:
                if (!m_pMeshTail)
                {
                    break;
                }

                m_pMeshTail->numFaces = ReadShort(pFile);

                m_pMeshTail->pFacesList = new short[m_pMeshTail->numFaces * 4];

                m_pMeshTail->pNormalsFacesList = new FVector3D[m_pMeshTail->numFaces];

                memset(m_pMeshTail->pNormalsFacesList, 0, sizeof(FVector3D) * m_pMeshTail->numFaces);

                for (i = 0; i < m_pMeshTail->numFaces; i ++)
                {
                    fread(&(m_pMeshTail->pFacesList[i * 4 + 2]), sizeof(short), 1, pFile);   // clock wise order
                    fread(&(m_pMeshTail->pFacesList[i * 4 + 1]), sizeof(short), 1, pFile);  // worth experiment'n
                    fread(&(m_pMeshTail->pFacesList[i * 4]), sizeof(short), 1, pFile);
                    fread(&(m_pMeshTail->pFacesList[i * 4 + 3]), sizeof(short), 1, pFile);  // face order
                }

                break;

            case 0x4130:
                if (!m_pMeshTail)
                {
                    break;
                }

                if (!m_pMeshTail->pFacesMatHead)
                {
                    m_pMeshTail->pFacesMatHead = new FacesMat();

                    m_pMeshTail->pFacesMatTail = m_pMeshTail->pFacesMatHead;
                }
                else
                {
                    m_pMeshTail->pFacesMatTail->pFacesMatNext = new FacesMat;

                    m_pMeshTail->pFacesMatTail = m_pMeshTail->pFacesMatTail->pFacesMatNext;
                }

                memset(m_pMeshTail->pFacesMatTail, 0, sizeof(FacesMat));

                i = 0;
                while ((ch = ReadChar(pFile)))
                {
                    m_pMeshTail->pFacesMatTail->name[i] = ch;
                    i ++;
                }
                m_pMeshTail->pFacesMatTail->name[i] = '\0';

                m_pMeshTail->pFacesMatTail->numEntries = ReadShort(pFile);

                m_pMeshTail->pFacesMatTail->pEntries = new short[m_pMeshTail->pFacesMatTail->numEntries];

                for (i = 0; i < m_pMeshTail->pFacesMatTail->numEntries; i ++)
                {
                    fread(&m_pMeshTail->pFacesMatTail->pEntries[i], sizeof(short), 1, pFile);
                }

                break;

            case 0x4140:
                if (!m_pMeshTail)
                {
                    break;
                }

                m_pMeshTail->numMappingCoords = ReadShort(pFile);

                m_pMeshTail->pMappingCoords = new FVector2D[m_pMeshTail->numMappingCoords];

                for (i = 0; i < m_pMeshTail->numMappingCoords; i ++)
                {
                    fread(&(m_pMeshTail->pMappingCoords[i].x), sizeof(float), 1, pFile);
                    fread(&(m_pMeshTail->pMappingCoords[i].y), sizeof(float), 1, pFile);
                }

                break;

            case 0x4160:
                if (!m_pMeshTail)
                {
                    break;
                }

                for (j = 0; j < 4; j ++)
                {
                    for (i = 0; i < 3; i ++)
                    {
                        fread(&(m_pMeshTail->localTransMatrix[j][i]), sizeof(float), 1, pFile);
                    }
                }
                m_pMeshTail->localTransMatrix[0][3] = 0.0f;
                m_pMeshTail->localTransMatrix[1][3] = 0.0f;
                m_pMeshTail->localTransMatrix[2][3] = 0.0f;
                m_pMeshTail->localTransMatrix[3][3] = 1.0f;

                break;

            case 0x4000:
                fseek(pFile, -6, SEEK_CUR);

                done = 1;

                break;

            default:
                DEBUGLOG("Object3D::ReadMesh -> Unknown chunk! (%x)\n", id);

                pos = ftell(pFile);
                if ((pos - 6) >= count)
                {
                    fseek(pFile, -6, SEEK_CUR);

                    done = 1;

                    break;
                }
                pos += (llen - 6);
                if (fseek(pFile, pos, SEEK_SET))
                {
                    done = 1;
                }

                break;
        }
    }
}

void Object3D::ReadMaterials(FILE *pFile, int len)
{
    bool            isAmbient  = false;
    bool            isDiffuse  = false;
    bool            isSpecular = false;
    char            ch;
    unsigned char   r;
    unsigned char   g;
    unsigned char   b;
    int             id;
    int             llen;
    int             done = 0;
    int             i;
    long            pos;
    long            count = ftell(pFile) + (len - 6);

    if (!m_pMaterialsHead)
    {
        m_pMaterialsHead = new Material();

        m_pMaterialsTail = m_pMaterialsHead;
    }
    else
    {
        m_pMaterialsTail->pMaterialNext = new Material();

        m_pMaterialsTail = m_pMaterialsTail->pMaterialNext;
    }

    memset(m_pMaterialsTail, 0, sizeof(Material));

    while (!done)
    {
        id = ReadShort(pFile);

        if (feof(pFile))
        {
            done = 1;

            break;
        }

        llen = ReadInt(pFile);

        switch (id)
        {
            case 0xa000:
                i = 0;
                while ((ch = ReadChar(pFile)) != 0)
                {
                    m_pMaterialsTail->name[i] = ch;
                    i ++;
                }
                m_pMaterialsTail->name[i] = '\0';

                break;

            case 0xa010:
                isDiffuse = false;
                isSpecular = false;
                isAmbient = true;

                m_pMaterialsTail->colorAmbient = 0;

                break;

            case 0xa020:
                isDiffuse = true;
                isSpecular = false;
                isAmbient = false;

                m_pMaterialsTail->colorDiffuse = 0;

                break;

            case 0xa030:
                isDiffuse = false;
                isSpecular = true;
                isAmbient = false;

                m_pMaterialsTail->colorSpecular = 0;

                break;

            case 0xa200:
                if (!m_pMaterialsTail->pTexturesHead)
                {
                    m_pMaterialsTail->pTexturesHead = new Texture();

                    m_pMaterialsTail->pTexturesTail = m_pMaterialsTail->pTexturesHead;
                }
                else
                {
                    m_pMaterialsTail->pTexturesTail->pTextureNext = new Texture();

                    m_pMaterialsTail->pTexturesTail = m_pMaterialsTail->pTexturesTail->pTextureNext;
                }

                memset(m_pMaterialsTail->pTexturesTail, 0, sizeof(Texture));

                break;

            case 0xa300:
                i = 0;
                while ((ch = ReadChar(pFile)))
                {
                    m_pMaterialsTail->pTexturesTail->name[i] = ch;
                    i ++;
                }
                m_pMaterialsTail->pTexturesTail->name[i] = '\0';

                break;

            case 0xa354:
                fread(&(m_pMaterialsTail->pTexturesTail->v), sizeof(float), 1, pFile);

                break;

            case 0xa356:
                fread(&(m_pMaterialsTail->pTexturesTail->u), sizeof(float), 1, pFile);

                break;

            case 0xa358:
                fread(&(m_pMaterialsTail->pTexturesTail->uOff), sizeof(float), 1, pFile);

                break;

            case 0xa35a:
                fread(&(m_pMaterialsTail->pTexturesTail->vOff), sizeof(float), 1, pFile);

                break;

            case 0xa35c:
                fread(&(m_pMaterialsTail->pTexturesTail->rot), sizeof(float), 1, pFile);

                break;

            case 0x0011:
                if (isDiffuse)
                {
                    fread(&r, sizeof(unsigned char), 1, pFile);
                    fread(&g, sizeof(unsigned char), 1, pFile);
                    fread(&b, sizeof(unsigned char), 1, pFile);

                    m_pMaterialsTail->colorDiffuse = (0xff << 24) | (b << 16) | (g << 8) | r;
                }
                else if (isAmbient)
                {
                    fread(&r, sizeof(unsigned char), 1, pFile);
                    fread(&g, sizeof(unsigned char), 1, pFile);
                    fread(&b, sizeof(unsigned char), 1, pFile);

                    m_pMaterialsTail->colorAmbient = (0xff << 24) | (b << 16) | (g << 8) | r;
                }
                else if (isSpecular)
                {
                    fread(&r, sizeof(unsigned char), 1, pFile);
                    fread(&g, sizeof(unsigned char), 1, pFile);
                    fread(&b, sizeof(unsigned char), 1, pFile);

                    m_pMaterialsTail->colorSpecular = (0xff << 24) | (b << 16) | (g << 8) | r;
                }

                break;

            default:
                DEBUGLOG("Object3D::ReadMaterials -> Unknown chunk! (%x)\n", id);

                pos = ftell(pFile);
                if ((pos - 6) >= count)
                {
                    fseek(pFile, -6, SEEK_CUR);

                    done = 1;

                    break;
                }
                pos += (llen - 6);
                if (fseek(pFile, pos, SEEK_SET))
                {
                    done = 1;
                }

                break;
        }
    }
}

void Object3D::CalcNormals()
{
    int         a;
    int         index1;
    int         index2;
    int         index3;
    int         *pVerticesCountFaces;
    FVector3D   vec1;
    FVector3D   vec2;
    FVector3D   vec3;
    Vector3D    vecCopy1;
    Vector3D    vecCopy2;
    Vector3D    vecCopy3;
    Vector3D    vecCopy4;
    Vector3D    vecCopy5;
    Mesh        *pMeshCurr = m_pMeshHead;

    if (pMeshCurr)
    {
        pVerticesCountFaces = new int[pMeshCurr->numVertices];

        do
        {
            memset(pVerticesCountFaces, 0, sizeof(int) * pMeshCurr->numVertices);

            for (a = 0; a < pMeshCurr->numFaces; a ++)
            {
                index1 = pMeshCurr->pFacesList[a * 4];
                pVerticesCountFaces[index1] ++;
                vec1 = pMeshCurr->pVerticesList[index1];

                index2 = pMeshCurr->pFacesList[a * 4 + 1];
                pVerticesCountFaces[index2] ++;
                vec2 = pMeshCurr->pVerticesList[index2];

                index3 = pMeshCurr->pFacesList[a * 4 + 2];
                pVerticesCountFaces[index3] ++;
                vec3 = pMeshCurr->pVerticesList[index3];

                vecCopy1.Set(vec1.x, vec1.y, vec1.z);
                vecCopy2.Set(vec2.x, vec2.y, vec2.z);
                vecCopy3.Set(vec3.x, vec3.y, vec3.z);

                Vector3D::SubVectors(&vecCopy2, &vecCopy2, &vecCopy1);
                Vector3D::SubVectors(&vecCopy3, &vecCopy3, &vecCopy1);
                Vector3D::CrossProductVectors(&vecCopy4, &vecCopy2, &vecCopy3);
                Vector3D::NormalizeVector(&vecCopy2, &vecCopy4);

                vec2.x = vecCopy2.m_x;
                vec2.y = vecCopy2.m_y;
                vec2.z = vecCopy2.m_z;

                pMeshCurr->pNormalsFacesList[a] = vec2;

                vecCopy5.Set(pMeshCurr->pNormalsVerticesList[index1].x, pMeshCurr->pNormalsVerticesList[index1].y, pMeshCurr->pNormalsVerticesList[index1].z);
                Vector3D::AddVectors(&vecCopy5, &vecCopy5, &vecCopy2);
                pMeshCurr->pNormalsVerticesList[index1].x = vecCopy5.m_x;
                pMeshCurr->pNormalsVerticesList[index1].y = vecCopy5.m_y;
                pMeshCurr->pNormalsVerticesList[index1].z = vecCopy5.m_z;

                vecCopy5.Set(pMeshCurr->pNormalsVerticesList[index2].x, pMeshCurr->pNormalsVerticesList[index2].y, pMeshCurr->pNormalsVerticesList[index2].z);
                Vector3D::AddVectors(&vecCopy5, &vecCopy5, &vecCopy2);
                pMeshCurr->pNormalsVerticesList[index2].x = vecCopy5.m_x;
                pMeshCurr->pNormalsVerticesList[index2].y = vecCopy5.m_y;
                pMeshCurr->pNormalsVerticesList[index2].z = vecCopy5.m_z;

                vecCopy5.Set(pMeshCurr->pNormalsVerticesList[index3].x, pMeshCurr->pNormalsVerticesList[index3].y, pMeshCurr->pNormalsVerticesList[index3].z);
                Vector3D::AddVectors(&vecCopy5, &vecCopy5, &vecCopy2);
                pMeshCurr->pNormalsVerticesList[index3].x = vecCopy5.m_x;
                pMeshCurr->pNormalsVerticesList[index3].y = vecCopy5.m_y;
                pMeshCurr->pNormalsVerticesList[index3].z = vecCopy5.m_z;
            }

            for (a = 0; a < pMeshCurr->numVertices; a ++)
            {
                vecCopy5.Set(pMeshCurr->pNormalsVerticesList[a].x, pMeshCurr->pNormalsVerticesList[a].y, pMeshCurr->pNormalsVerticesList[a].z);
                Vector3D::MultiplyVectorScalar(&vecCopy5, &vecCopy5, pVerticesCountFaces[a]);
                pMeshCurr->pNormalsVerticesList[a].x = vecCopy5.m_x;
                pMeshCurr->pNormalsVerticesList[a].y = vecCopy5.m_y;
                pMeshCurr->pNormalsVerticesList[a].z = vecCopy5.m_z;
            }

            pMeshCurr = pMeshCurr->pMeshNext;
        } while (pMeshCurr);

        delete pVerticesCountFaces;
    }
}
