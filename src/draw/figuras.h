#pragma once 
#include "../math/vectores.h"
#include "../color/colores.h"
#include "stdint.h"

typedef enum _tipos_fig
{
    LINEA = 1 << 0,
    CURVA = 1 << 1,
    CUADRO= 1 << 2,
    TRIAN = 1 << 3,
    CIRC  =  1 << 4
} TypeFig;

typedef struct _cuadro{

    Vec2 pos;
    int w;
    int h;
    char offset_mem1[8];
    Color color;
    TypeFig type;
} Cuadro;

typedef struct _circulo

{
    Vec2 pos;
    float r;
    int vert;
    char offset_mem1[8];
    Color color;
    TypeFig type;
}Circulo;


typedef struct _triangulo
{
    Vec2 p1;
    Vec2 p2;
    Vec2 p3;
    Color Color;
    TypeFig type;
}Triangulo;

typedef struct _linea
{
    Vec2 p1;
    Vec2 p2;
    char offset_mem1[8];
    Color color;
    TypeFig type;
}Linea;

typedef struct _curva
{
    Vec2 p1;
    Vec2 p2;
    Vec2 p3;
    Color color;
    TypeFig type;
}Curva;

typedef struct _figdummy

{
     char offset[28];
    // Vec2 p1;
    // Vec2 p2;
    // Vec2 p3;
    // Color color;
    TypeFig type;
   

} FigComun;



typedef union _figuras
{
    FigComun data;
    Cuadro cuadro;
    Circulo circulo;
    Triangulo trirangulo;
    Linea linea;
    Curva curva;
}Figuras;

void draw_figuras(Figuras *fig);

