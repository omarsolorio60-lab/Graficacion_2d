#pragma once
#include <stdint.h>
#include "../math/vectores.h"

void draw_pixel(int x, int y, uint32_t color);
void draw_linea(int x0, int y0, int x1, int y1, uint32_t color);
void draw_triangulo(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_cuadro(int x, int y, int w, int h, uint32_t color);
void draw_circulo(int x0, int y0, float radio, uint32_t color, int numvertices);

void fill_flat_bottom(Vec2 p1, Vec2 p2, Vec2 c, uint32_t color);
void fill_flat_top(Vec2 p1, Vec2 p2, Vec2 c, uint32_t color);
void fill_triangulo(Vec2 v1, Vec2 v2, Vec2 v3, uint32_t color);
//curvas 
// void draw_linea_lerp(Vec2 p1,Vec2 p2, uint32_t color);
void draw_curva(Vec2 p1, Vec2 p2, Vec2 p3, uint32_t color);

//rellenno practia 2
void relleno_limites_iter(int sx, int sy, uint32_t fill_color, uint32_t boundary_color, int usar8);

static inline void fill_cuadro(Vec2 pos, int w, int h, uint32_t color, uint32_t contorno)
{
    if (w <= 0 || h <= 0) return;

    // Relleno por scanlines
    for (int y = pos.unpack.y; y < pos.unpack.y + h; ++y) {
        // si prefieres incluir el último píxel usa x+w-1
        draw_linea(pos.unpack.x, y, pos.unpack.x + w - 1, y, color);
    }

    // Borde correcto: (x, y, w, h)
    draw_cuadro(pos.unpack.x, pos.unpack.y, w, h, contorno);
}