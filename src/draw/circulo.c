#include "draw.h"
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void draw_circulo(int x0, int y0, float radio, uint32_t color, int numvertices){
    #ifndef CIRCULO_TRIG
    for(int i=0; i<numvertices; i++){

        float ang_1 = (i * 2 * M_PI) / numvertices;
        float ang_2 = ((i + 1) %numvertices * 2 * M_PI) / numvertices;
        float x = x0 + cos(ang_1) * radio;
        float y = y0+sin(ang_1) * radio;
        float xs = x0 + cos(ang_2)* radio;
        float ys = y0+sin(ang_2) * radio;
        draw_linea(round(x), round(y), round (xs), round(ys), color);
    }
    #else 

    int x = 0;
    int y =radio;
    int p = 1- radio;
    while( x <=y){
        draw_pixel(x0 + x, y0 + y, color);
        draw_pixel(x0 - x, y0 + y, color);
        draw_pixel(x0 + x, y0 - y, color);
        draw_pixel(x0 - x, y0 - y, color);
        draw_pixel(x0 + y, y0 + x, color);
        draw_pixel(x0 - y, y0 + x, color);
        draw_pixel(x0 + y, y0 - x, color);
        draw_pixel(x0 - y, y0 - x, color);
        x++;
        if(p<0) p += 2*x + 1;
        else {
            y--;
            p += 2*(x-y) + 1;
        }
    }

    #endif
}