#ifndef _ENGINE3D_H_
#define _ENGINE3D_H_

#include "List.h"
#include "MemoryPool.h"
#include "Engine3D/Color.h"
#include "Engine3D/Camera.h"
#include "Engine3D/Vector2D.h"
#include "Engine3D/Vector3D.h"
#include "Engine3D/Light.h"
#include "Engine3D/ExtVector3D.h"
#include "Engine3D/Primitive3D.h"

class Point3D;
class Line3D;
class Triangle3D;
class Camera;
class Light;
class Primitive3D;
class ExtVector3D;

class Engine3D
{
public:
    enum ShadingTypes
    {
        SHADING_TYPE_FLAT,
        SHADING_TYPE_GOURAUD
    };

    Color           m_backgroundColor;
    ShadingTypes    m_shading;

    // ------------------------------------------------------------------

    Engine3D(const Color *pBackgroundColor, const ShadingTypes shading);
    ~Engine3D();

    void            Reset();

    bool            AddVertex3D(ExtVector3D *pVertex);
    int             GetNumVertexes3D();
    bool            RemoveVertex3D(ExtVector3D *pVertex);

    bool            AddPrimitive3D(Primitive3D *pPrimitive);
    int             GetNumPrimitives3D();
    bool            RemovePrimitive3D(Primitive3D *pPrimitive);

    void            AddLight(Light *pLight);

    void            SetCamera(Camera *pCamera);

    void            ClearBackground();
    void            ClearZBuffer();
    void            ClearListVertexes();
    void            ClearListPrimitives();

    void            Render();

private:
    MemoryPool      *m_pMemoryPoolVertexes;

    int             m_numVertexesClipped;
    List            m_listVertexesOriginal;
    List            m_listVertexesProcessed;
    List            m_listVertexesClipped;
    List            m_listPrimitives;

    Camera          *m_pCamera;

    Light           *m_pLightAmbient;
    Light           *m_pLightOther;

    Color           m_colorTemp;
    Color           m_colorTemp2;
    Color           m_colorInterpolated;
    Vector2D        m_uvInterpolated;
    Vector3D        m_renderOldP;
    Vector3D        m_renderOldM;
    Vector3D        m_renderOldN;
    Vector3D        m_renderNewP;
    Vector3D        m_renderNewM;
    Vector3D        m_renderNewN;
    Vector3D        m_renderA;
    Vector3D        m_renderB;
    Vector3D        m_renderC;
    Vector3D        m_renderD;
    Vector3D        m_vectorTemp;

    // ------------------------------------------------------------------

    void            CalcNormalsAndGouraudShading();
    void            ClipPoints();
    void            ClipLines();
    void            ClipTriangles();
    void            TrasformVertexes3Dto2D();
    void            Draw();
    void            DrawPoint3D(Point3D *pPoint);
    void            DrawLine3D(Line3D *pLine);
    void            DrawLinePoint3D(Line3D *pLine, const int offset, const float rate, const float z, const int firstIndex, const int secondIndex);
    bool            DrawTriangle3D(Triangle3D *pTriangle);
    void            DrawTrianglePoint3D(Triangle3D *pTriangle, const int offset, const int firstIndex, const int secondIndex, const int thirdIndex,
                                        const float a, const float b, const float c);
};

extern Engine3D *g_pEngine3D;

#endif
