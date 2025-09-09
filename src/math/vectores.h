#pragma once 
#include "../color/colores.h"
typedef union _vec2
{
    float data[2];

    struct 
    {
        float x;
        float y;
        float c;
    } unpack;
}Vec2;

typedef union _vec3
{
    float data[3];
    struct
    {
        float x;
        float y;
        float z;
    } unpack;
}Vec3;

float distanciav2(Vec2 p1, Vec2 p2);
void draw_cuadratica(Vec2 P1, Vec2 P2, Vec2 P3, uint32_t color, float dt);
void draw_cubica(Vec2 P1, Vec2 P2, Vec2 P3, Vec2 P4, uint32_t color, float dt);
void draw_lerp(Vec2 P1, Vec2 P2, uint32_t color, float dt);

                                  