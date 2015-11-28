#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "utils.h"
#include "List.h"
#include "Engine3D/Vector3D.h"
#include "Engine3D/Matrix.h"

class Object3D
{
public:
    Vector3D            m_pos;
    Vector3D            m_angle;
    Vector3D            m_scale;

    // ------------------------------------------------------------------

    Object3D();
    ~Object3D();

    bool                Load3DS(const char *pFileName);

    void                PrintMeshInfo();
    void                PrintMaterialsInfo();

    void                UpdateMatrix();

    void                Render();

    void                InitVertexesAndTriangles();
    void                DestroyVertexesAndTriangles();

private:
    typedef struct
    {
        float           x;
        float           y;
    } FVector2D;

    typedef struct
    {
        float           x;
        float           y;
        float           z;
    } FVector3D;

    typedef struct _Texture
    {
        char            name[256];

        float           u;
        float           v;
        float           uOff;
        float           vOff;
        float           rot;

        _Texture        *pTextureNext;
    } Texture;

    typedef struct _Material
    {
        char            name[200];

        unsigned int    colorAmbient;
        unsigned int    colorDiffuse;
        unsigned int    colorSpecular;

        _Texture        *pTexturesHead;
        _Texture        *pTexturesTail;

        _Material       *pMaterialNext;
    } Material;

    typedef struct _FacesMat
    {
        char            name[200];

        int             numEntries;
        short           *pEntries;

        _FacesMat       *pFacesMatNext;
    } FacesMat;

    typedef struct _Mesh
    {
        char            name[200];

        int             numVertices;
        FVector3D       *pVerticesList;

        int             numFaces;
        short           *pFacesList;

        FVector3D       *pNormalsVerticesList;
        FVector3D       *pNormalsFacesList;

        int             numMappingCoords;
        FVector2D       *pMappingCoords;

        float           localTransMatrix[4][4];

        _Mesh           *pMeshNext;

        FacesMat        *pFacesMatHead;
        FacesMat        *pFacesMatTail;
    } Mesh;

    Mesh                *m_pMeshHead;
    Mesh                *m_pMeshTail;

    Material            *m_pMaterialsHead;
    Material            *m_pMaterialsTail;

    List                m_listVertexes;
    List                m_listTriangles3D;

    Matrix              m_matrix;

    // ------------------------------------------------------------------

    void                DestroyMeshAndMaterials();

    bool                ReadPrimaryChunk(FILE *pFile);
    bool                Read3DS(FILE *pFile);
    void                ReadObject(FILE *pFile, int len);
    void                ReadMesh(FILE *pFile, int len);
    void                ReadMaterials(FILE *pFile, int len);
    void                CalcNormals();
};

#endif
