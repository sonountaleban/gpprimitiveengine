#ifndef _PRIMITIVE3D_H_
#define _PRIMITIVE3D_H_

#include "Engine3D/ExtVector3D.h"

class ExtVector3D;

class Primitive3D
{
    friend class Engine3D;

public:
    enum Types
    {
        TYPE_NONE = -1,
        TYPE_POINT3D = 0,
        TYPE_LINE3D,
        TYPE_TRIANGLE3D
    };

    virtual ~Primitive3D();

    bool            IsVisible();

    Types           GetType();

    void            AddOffsetToListNumVertexes(int offset);
    void            ResetListNumVertexes();

protected:
    Types           m_type;

    int             m_numVertexes;
    int             m_numMaxVertexesClipped;
    int             m_numVertexesClipped;
    int             *m_pListNumVertexes;
    int             *m_pListNumVertexesCopy;
    ExtVector3D     **m_ppListVertexesProcessed;
    ExtVector3D     **m_ppListVertexesClipped;

    // ------------------------------------------------------------------

    void            AddVertexClipped(ExtVector3D *pVertex);
    void            RemoveAllVertexesClipped();
    void            TrasformAllVertexesClippedTo2D(const int screenCenterX, const int screenCenterY, const float screenDistX, const float screenDistY);
};

#endif
