#include "draw.h"
#include <stdlib.h>

void fill_flat_bottom(Vec2 p1, Vec2 p2, Vec2 c, uint32_t color)
{
    //calcular la pendiente inversa para x inicial(p1) y final (c)
    float mi = (p2.unpack.x-p1.unpack.x)/(p2.unpack.y-p1.unpack.y); // pendiente lado izq
    float mf = (c.unpack.x-p1.unpack.x)/(c.unpack.y-p1.unpack.y); // pendiente lado derecho
    float xi = p1.unpack.x;
    float xf = p1.unpack.x;

    for (int y= (int)p1.unpack.y; y <= (int)p2.unpack.y; y++)
    {
        draw_linea(xi,y,xf,y,color);
        xi += mi;
        xf += mf;
    }
    
}
void fill_flat_top(Vec2 p1, Vec2 c, Vec2 p2, uint32_t color)
{

    float mi = (p2.unpack.x-p1.unpack.x)/(p2.unpack.y-p1.unpack.y); // pendiente lado izq
    float mf = (p2.unpack.x-c.unpack.x)/(p2.unpack.y-c.unpack.y); //pendiente lado derecho
    float xi = p2.unpack.x;
    float xf = p2.unpack.x;

    for(int y = (int)c.unpack.y; y > (int)p1.unpack.y; y--)
    {
        draw_linea(xi,y,xf, y, color);
        xi -=mi;
        xf -=mf;
    }
    

}

void fill_triangulo(Vec2 v1, Vec2 v2, Vec2 v3, uint32_t color)
{
    // 1) Ordenar por Y ascendente (v1 arriba, v3 abajo)
    if (v1.unpack.y > v2.unpack.y) { Vec2 t = v1; v1 = v2; v2 = t; }
    if (v1.unpack.y > v3.unpack.y) { Vec2 t = v1; v1 = v3; v3 = t; }
    if (v2.unpack.y > v3.unpack.y) { Vec2 t = v2; v2 = v3; v3 = t; }

    // 2) Casos base plana
    if ((int)v2.unpack.y == (int)v3.unpack.y) {
        // Base plana abajo
        fill_flat_bottom(v1, v2, v3, color);
    }
    else if ((int)v1.unpack.y == (int)v2.unpack.y) {
        // Base plana arriba
        fill_flat_top(v1, v2, v3, color);
    }
    else {
        // 3) Caso general: dividir en dos con v4
        Vec2 v4;
        v4.unpack.y = v2.unpack.y;

        float t = (v2.unpack.y - v1.unpack.y) / (v3.unpack.y - v1.unpack.y);
        v4.unpack.x = v1.unpack.x + t * (v3.unpack.x - v1.unpack.x);

        fill_flat_bottom(v1, v2, v4, color);
        fill_flat_top(v2, v4, v3, color);
    }
}