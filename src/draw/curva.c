#include "draw.h"
#include <math.h>
#include <stdlib.h>
#include "../math/lerp.h"
#include "../math/vectores.h"


void draw_curva(Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color)
{
    float dt = 0.01f;             

    for (float t = 0.0f; t <= 1.0f; t += dt) {
        Vec2 p = lerp2v2(p1, p2,p3, t);
        int x = (int)lroundf(p.unpack.x);
        int y = (int)lroundf(p.unpack.y);
        draw_pixel(x, y, color);
    }
}