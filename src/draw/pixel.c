#include "draw.h"
#include "../global.h"

void draw_pixel(int x, int y, uint32_t color)
{
    if((x<0 || y<0) ) return;
    if( x>= estadosrender.ven_width || y >= estadosrender.ven_height) return;
    estadosrender.color_buffer[y*estadosrender.ven_width+x] = color;
}