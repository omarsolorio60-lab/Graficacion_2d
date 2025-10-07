#include "figuras.h"
#include "draw.h"
#include <math.h>
#include "../global.h"

// ----------------- helpers de relleno -----------------

static void draw_cuadro_relleno(int x,int y,int w,int h,uint32_t col){
    if (w <= 0 || h <= 0) return;
    int x0 = x;
    int x1 = x + w - 1;
    int y0 = y;
    int y1 = y + h - 1;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 >= estadosrender.ven_width)  x1 = estadosrender.ven_width - 1;
    if (y1 >= estadosrender.ven_height) y1 = estadosrender.ven_height - 1;

    for (int j = y0; j <= y1; ++j){
        draw_linea(x0, j, x1, j, col);
    }
}

static void draw_circulo_relleno(int cx,int cy,int r,uint32_t col){
    if (r <= 0) return;
    int y_min = -r, y_max = r;
    for (int dy = y_min; dy <= y_max; ++dy){
        int yy = cy + dy;
        if (yy < 0 || yy >= estadosrender.ven_height) continue;

        float inside = (float)r*(float)r - (float)dy*(float)dy;
        if (inside < 0.0f) continue; // por redondeos
        int xspan = (int)floorf(sqrtf(inside));
        int x0 = cx - xspan;
        int x1 = cx + xspan;

        if (x1 < 0 || x0 >= estadosrender.ven_width) continue;
        if (x0 < 0) x0 = 0;
        if (x1 >= estadosrender.ven_width) x1 = estadosrender.ven_width - 1;

        draw_linea(x0, yy, x1, yy, col);
    }
}

// edge function para triangulo (half-space)
static inline int edge(int ax,int ay,int bx,int by,int px,int py){
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

static void draw_triangulo_relleno(int ax,int ay,int bx,int by,int cx,int cy,uint32_t col){
    // bounding box
    int minx = ax; if (bx < minx) minx = bx; if (cx < minx) minx = cx;
    int miny = ay; if (by < miny) miny = by; if (cy < miny) miny = cy;
    int maxx = ax; if (bx > maxx) maxx = bx; if (cx > maxx) maxx = cx;
    int maxy = ay; if (by > maxy) maxy = by; if (cy > maxy) maxy = cy;

    if (minx < 0) minx = 0;
    if (miny < 0) miny = 0;
    if (maxx >= estadosrender.ven_width)  maxx = estadosrender.ven_width - 1;
    if (maxy >= estadosrender.ven_height) maxy = estadosrender.ven_height - 1;

    int area2 = edge(ax,ay,bx,by,cx,cy);
    if (area2 == 0) return; // degenerado

    // Recorremos por filas (scanlines)
    for (int y = miny; y <= maxy; ++y){
        for (int x = minx; x <= maxx; ++x){
            int w0 = edge(bx,by,cx,cy,x,y);
            int w1 = edge(cx,cy,ax,ay,x,y);
            int w2 = edge(ax,ay,bx,by,x,y);

            if ((area2 > 0 && w0 >= 0 && w1 >= 0 && w2 >= 0) ||
                (area2 < 0 && w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                draw_pixel(x,y,col);
            }
        }
    }
}

//draw de figuras (relleno + borde) 

void draw_figuras(Figuras *fig){
    switch(fig->data.type){
        case CUADRO:{
            const Cuadro q = fig->cuadro;
            // Relleno
            draw_cuadro_relleno(q.pos.unpack.x, q.pos.unpack.y, q.w, q.h, q.fill.hex);
            // Borde
            int x  = q.pos.unpack.x, y = q.pos.unpack.y;
            int x1 = x + q.w - 1,   y1 = y + q.h - 1;
            uint32_t col = q.color.hex;
            draw_linea(x, y,  x1, y,  col);
            draw_linea(x1,y,  x1,y1, col);
            draw_linea(x1,y1, x, y1, col);
            draw_linea(x, y1, x, y,  col);
        } break;

        case CIRC:{
            const Circulo c = fig->circulo;
            // Relleno
            draw_circulo_relleno(c.pos.unpack.x, c.pos.unpack.y, c.r, c.fill.hex);
            // Borde
            draw_circulo(c.pos.unpack.x, c.pos.unpack.y, c.r, c.color.hex, c.vert);
        } break;

        case TRIAN:{
            const Triangulo t = fig->trirangulo;
            // Relleno
            draw_triangulo_relleno(
                t.p1.unpack.x,t.p1.unpack.y,
                t.p2.unpack.x,t.p2.unpack.y,
                t.p3.unpack.x,t.p3.unpack.y,
                t.fill.hex
            );
            // Borde
            uint32_t col = t.Color.hex;
            draw_linea(t.p1.unpack.x,t.p1.unpack.y,t.p2.unpack.x,t.p2.unpack.y,col);
            draw_linea(t.p2.unpack.x,t.p2.unpack.y,t.p3.unpack.x,t.p3.unpack.y,col);
            draw_linea(t.p3.unpack.x,t.p3.unpack.y,t.p1.unpack.x,t.p1.unpack.y,col);
        } break;

        case LINEA:{
            const Linea f = fig->linea;
            draw_circulo(f.p1.unpack.x,f.p1.unpack.y,5.f,0x0000ffff,32);
            draw_circulo(f.p2.unpack.x,f.p2.unpack.y,5.f,0x0000ffff,32);
            draw_linea(f.p1.unpack.x,f.p1.unpack.y,f.p2.unpack.x,f.p2.unpack.y, f.color.hex);
        } break;

        case CURVA:{
            const Curva c = fig->curva;
            draw_circulo(c.p1.unpack.x,c.p1.unpack.y,5.f,0x0000ffff,32);
            draw_circulo(c.p2.unpack.x,c.p2.unpack.y,5.f,0xff0000ff,32);
            draw_circulo(c.p3.unpack.x,c.p3.unpack.y,5.f,0x0000ffff,32);
            draw_curva(c.p1,c.p2,c.p3,c.color.hex);
        } break;

        default: break;
    }
}