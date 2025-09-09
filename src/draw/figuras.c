#include "figuras.h"
#include "draw.h"
#include <stdio.h>

void draw_figuras(Figuras *fig)
{
    switch(fig->data.type)
    {
        case LINEA:
        {
            Linea f = fig->linea;
            draw_circulo(f.p1.unpack.x,f.p1.unpack.y,5.f,0x0000ffff,32);
            draw_circulo(f.p2.unpack.x,f.p2.unpack.y,5.f,0x0000ffff,32);
            draw_linea(f.p1.unpack.x,f.p1.unpack.y,f.p2.unpack.x,f.p2.unpack.y, f.color.hex);
        }
        break;

        case CURVA:
        {
            Curva c = fig -> curva;
            draw_circulo(c.p1.unpack.x,c.p1.unpack.y,5.f,0x0000ffff,32);
            draw_circulo(c.p2.unpack.x,c.p2.unpack.y,5.f,0xff0000ff,32);
            draw_circulo(c.p3.unpack.x,c.p3.unpack.y,5.f,0x0000ffff,32);
            draw_curva(c.p1,c.p2,c.p3,c.color.hex);
        }
        break;
        case CUADRO:
        break;
        case TRIAN:
        break;
        case CIRC:
        {
            Circulo c = fig->circulo;
            draw_circulo(c.pos.unpack.x,c.pos.unpack.y,c.r,c.color.hex,c.vert);

        }
        break;
        default:
        break;
        

        

    }
}
