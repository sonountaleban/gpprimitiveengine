#ifndef _LINE3D_H_
#define _LINE3D_H_

#include "Engine3D/Primitive3D.h"

class Line3D : public Primitive3D
{
    friend class Engine3D;

public:
    Line3D(const int vertex1, const int vertex2);
    ~Line3D();

private:

};

#endif
