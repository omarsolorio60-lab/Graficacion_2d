#include "vectores.h"
#include <math.h>
#include "lerp.h"
#include "../draw/draw.h"


void draw_cuadratica(Vec2 P1, Vec2 P2, Vec2 P3, uint32_t color, float dt)
{
    for (float t = 0.0f; t <= 1.0f; t += dt) {
        Vec2  p = lerp2v2(P1, P2, P3, t);

        int x = (int)lroundf(p.unpack.x);
        int y = (int)lroundf(p.unpack.y);

        draw_pixel(x, y, color);
    }
}

void draw_cubica(Vec2 P1, Vec2 P2, Vec2 P3, Vec2 P4, uint32_t color, float dt)
{
    for (float t = 0.0f; t <= 1.0f; t += dt) {
        Vec2  p = lerp2v3(P1, P2, P3, P4, t);
        

        int x = (int)lroundf(p.unpack.x);
        int y = (int)lroundf(p.unpack.y);
        draw_pixel(x, y, color);
    }
}

void draw_lerp(Vec2 P1, Vec2 P2, uint32_t color, float dt)
{
    for (float t = 0.0f; t <= 1.0f; t += dt) {
        Vec2 p = lerp2v(P1, P2, t);   

        int x = (int)(p.unpack.x + 0.5f);
        int y = (int)(p.unpack.y + 0.5f);

        draw_pixel(x, y, color);     
    }
}





                                  