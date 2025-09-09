#include "vectores.h"
#include "lerp.h"

Vec2 lerp2v3 (Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4, float t)
{
    Vec2 resultado;

    float u = 1.0f -t; //(1-t)

    float a = u * u * u; //(1-t)^3
    float b = 3 * u * u * t; //3(1-t)
    float c = 3 * u * t * t; //3(1-t)t^2
    float d = t * t * t; // t^3

    resultado.unpack.x = a * p1.unpack.x + b * p2.unpack.x + c * p3.unpack.x + d * p4.unpack.x;
    resultado.unpack.y = a * p1.unpack.y + b * p2.unpack.y + c * p3.unpack.y + d * p4.unpack.y;

    return resultado;
}
