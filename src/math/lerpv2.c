#include "vectores.h"
#include "lerp.h"

Vec2 lerp2v(Vec2 p1, Vec2 p2, float t)
{
    Vec2 resultado;

    resultado.unpack.x = p1.unpack.x + (p2.unpack.x - p1.unpack.x) * t;
    resultado.unpack.y = p1.unpack.y + (p2.unpack.y - p1.unpack.y) * t;

    return resultado;
}