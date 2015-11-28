#ifndef _TRIANGLE3D_H_
#define _TRIANGLE3D_H_

#include "Engine3D/Plane3D.h"
#include "Engine3D/Primitive3D.h"

class Triangle3D : public Primitive3D
{
    friend class Engine3D;

public:
    bool    m_culling;

    // ------------------------------------------------------------------

    Triangle3D(const int textureIndex, const int vertex1, const int vertex2, const int vertex3);
    ~Triangle3D();

    bool    IsVisible();

private:
    int     m_textureIndex;
    Plane3D m_plane;

    // ------------------------------------------------------------------

    void    AddFirstVertexClipped();
};

#endif
