#include "vectores.h"
#include "lerp.h"

Vec2 lerp2v2 (Vec2 p1, Vec2 p2, Vec2 p3, float t){

    Vec2 resultado;

    float u = 1.0f - t; // (1-t)
    
    float k1 = u * u; // (1-t)^2
    float k2 = 2*u*t; // 2(1-t)t
    float k3 = t*t; //t^2

    resultado.unpack.x = k1 * p1.unpack.x + k2 * p2.unpack.x + k3 * p3.unpack.x;
    resultado.unpack.y = k1 * p1.unpack.y + k2 * p2.unpack.y + k3 * p3.unpack.y;

    return resultado;


}