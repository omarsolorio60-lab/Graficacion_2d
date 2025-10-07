#include <stdint.h>
#include "figuras.h"
#include "../global.h"

void relleno_limites_iter(int sx, int sy, uint32_t fill_color,
                          uint32_t boundary_color, int usar8);

static inline int in_bounds(int x, int y) {
    return x >= 0 && y >= 0 &&
           x < estadosrender.ven_width &&
           y < estadosrender.ven_height;
}

// semillas
static void seed_cuadro(const Cuadro *q, int *sx, int *sy) {
    *sx = q->pos.unpack.x + (q->w / 2);
    *sy = q->pos.unpack.y + (q->h / 2);
}
static void seed_triangulo(const Triangulo *t, int *sx, int *sy) {
    *sx = (t->p1.unpack.x + t->p2.unpack.x + t->p3.unpack.x) / 3;
    *sy = (t->p1.unpack.y + t->p2.unpack.y + t->p3.unpack.y) / 3;
}
static void seed_circulo(const Circulo *c, int *sx, int *sy) {
    *sx = c->pos.unpack.x;
    *sy = c->pos.unpack.y;
}

// color de borde
static uint32_t color_borde(const Figuras* f) {
    switch (f->data.type) {
        case CUADRO: return f->cuadro.color.hex;
        case TRIAN:  return f->trirangulo.Color.hex;
        case CIRC:   return f->circulo.color.hex;
        default:     return 0xFFFFFFFFu;
    }
}

// color de relleno por figura
static uint32_t color_relleno(const Figuras* f) {
    switch (f->data.type) {
        case CUADRO: return f->cuadro.fill.hex;
        case TRIAN:  return f->trirangulo.fill.hex;
        case CIRC:   return f->circulo.fill.hex;
        default:     return 0x00000000u;
    }
}

void fill_figura(const Figuras* f, uint32_t _ignored, int usar8) {
    if (!f) return;

    int sx = -1, sy = -1;
    switch (f->data.type) {
        case CUADRO: { const Cuadro *q = &f->cuadro;        seed_cuadro(q, &sx, &sy); } break;
        case TRIAN:  { const Triangulo *t = &f->trirangulo; seed_triangulo(t, &sx, &sy); } break;
        case CIRC:   { const Circulo *c = &f->circulo;      seed_circulo(c, &sx, &sy); } break;
        default: return; // línea/curva no se rellenan aquí
    }

    if (!in_bounds(sx, sy)) return;

    const uint32_t boundary = color_borde(f);
    const uint32_t fill     = color_relleno(f);

    // el borde debe estar ya dibujado con 'boundary'
    relleno_limites_iter(sx, sy, fill, boundary, usar8);
}
