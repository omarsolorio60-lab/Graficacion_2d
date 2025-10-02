#include <stdlib.h>
#include <stdint.h>
#include "../global.h"
#include "../draw/draw.h"          // draw_pixel(...)
#include "../estructuras/pila.h"   // pila genérica

typedef struct {
    int x, y;
} Punto;

static inline int in_bounds(int x, int y) {
    return x >= 0 && y >= 0 && x < estadosrender.ven_width && y < estadosrender.ven_height;
}

static inline uint32_t get_color_pixel(int x, int y) {
    return estadosrender.color_buffer[y * estadosrender.ven_width + x];
}

void relleno_limites_iter(int sx, int sy, uint32_t fill_color, uint32_t boundary_color, int usar8)
{
    if (!in_bounds(sx, sy)) return;

    uint32_t cur = get_color_pixel(sx, sy);
    if (cur == boundary_color || cur == fill_color) return;

    Pila pila;
    pila_init(&pila);

    // push semilla
    Punto *seed = (Punto*)malloc(sizeof(Punto));
    if (!seed) return;
    seed->x = sx; seed->y = sy;
    pila_push(&pila, seed);

    while (!pila_vacia(&pila)) {
        Punto *p = (Punto*)pila_pop(&pila);
        if (!p) continue;

        int x = p->x, y = p->y;
        free(p); // ya no lo necesitamos

        if (!in_bounds(x, y)) continue;

        uint32_t c = get_color_pixel(x, y);
        if (c == boundary_color || c == fill_color) continue;

        draw_pixel(x, y, fill_color);

        // 4-conexiones
        const int dx4[4] = { 0, 1, 0,-1};
        const int dy4[4] = {-1, 0, 1, 0};

        // 8-conexiones (opcional)
        const int dx8[8] = { 0, 1, 0,-1,  1, 1,-1,-1};
        const int dy8[8] = {-1, 0, 1, 0, -1, 1,  1,-1};

        int n = usar8 ? 8 : 4;
        for (int i = 0; i < n; i++) {
            int nx = x + (usar8 ? dx8[i] : dx4[i]);
            int ny = y + (usar8 ? dy8[i] : dy4[i]);
            if (!in_bounds(nx, ny)) continue;

            uint32_t cc = get_color_pixel(nx, ny);
            if (cc == boundary_color || cc == fill_color) continue;

            Punto *q = (Punto*)malloc(sizeof(Punto));
            if (!q) continue; // si falla malloc, simplemente omitimos
            q->x = nx; q->y = ny;
            pila_push(&pila, q);
        }
    }

    pila_free(&pila);
}
