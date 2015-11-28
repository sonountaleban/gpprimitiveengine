#ifndef _POINT3D_H_
#define _POINT3D_H_

#include "Engine3D/Primitive3D.h"

class Point3D : public Primitive3D
{
    friend class Engine3D;

public:
    Point3D(const int vertex);
    ~Point3D();

private:

};

#endif
