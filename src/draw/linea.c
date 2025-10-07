#include "linea.h"
#include "draw.h"
#include <stdlib.h>
#include <math.h>


void draw_linea(int x0, int y0, int x1, int y1, uint32_t color){

    #if LINEA_BRUTA
        fuerza_bruta(x0,y0,x1,y1,color);
    #endif

    #if LINEA_DDA
        linea_dda(x0,y0,x1,y1,color);
    #endif
    #if LINEA_BRUTA
        linea_brhm(x0,y0,x1,y1,color);
    #endif
}

void fuerza_bruta(int x0, int y0, int x1, int y1, uint32_t color)

{

    int y;
    float dx = abs((float)(x1-x0));
    float dy = abs((float)(y1-y0));
    
    for(int x=x0; x<=x1; ++x){

        y = y0 +dy * ((x-x0)/dx);
        draw_pixel(x,y,color);
    }

}

void linea_dda(int x0, int y0, int x1, int y1, uint32_t color)

{
    int dx = (x1-x0);
    int dy = (y1-y0);
    int largo = ( abs (dx) > abs (dy)) ? abs (dx): abs(dy);
    
    float inc_x = dx / (float) largo;
    float inc_y = dy / (float) largo;
    float x = x0;
    float y = y0;

    for(int i=0; i<largo; i++)
    {
        draw_pixel(x, y, color);
        x+=inc_x;
        y+=inc_y;
    }
}

void linea_brhm(int x0, int y0, int x1, int y1, uint32_t color)
{
    int dx = abs(x1-x0);
    int sx = (x0 < x1) ? 1 : -1;
    int dy  = -abs(y1-y0);
    int sy = (y0 < y1) ? 1 : -1;
    int error = dx + dy;
    int x = x0;
    int y = y0;

    while(1)
    {
        draw_pixel(x,y,color);
        if(x == x1 && y == y1) break;
        int e2 = 2 * error;
        if (e2>= dy){
            error = error + dy;

            x+= sx;
        }
        if (e2 <= dx){
            error = error + dx;
            y += sy;
        }
    }

}

