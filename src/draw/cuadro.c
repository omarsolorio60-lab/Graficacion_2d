#include "draw.h"
#include <stdlib.h>


void draw_cuadro(int x, int y, int w, int h, uint32_t color){
    for(int i = 0; i < w; ++i){
        draw_pixel(x + i, y, color); // linea arriba
        draw_pixel(x + i, y + h - 1, color); // linea abajo
    }
    for(int j = 0; j < h; ++j){
        draw_pixel(x, y + j, color); // linea izquierda
        draw_pixel(x + w - 1, y + j, color); // linea derecha
    }


    
}