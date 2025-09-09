#include "vectores.h"
#include <math.h>

float distanciav2(Vec2 p1, Vec2 p2)
{
    return sqrt((p2.unpack.x-p1.unpack.x)*(p2.unpack.x-p1.unpack.x)
                +
                (p2.unpack.y-p1.unpack.y)*(p2.unpack.y-p1.unpack.y));
}