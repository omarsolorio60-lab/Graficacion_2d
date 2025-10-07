#pragma once
#include <stdint.h>
#include <SDL3/SDL.h>
#include "color/colores.h"
#include "draw/figuras.h"

typedef struct _estado
{
    SDL_Renderer *renderer;
    SDL_Texture *textura;
    SDL_Event evento;
    uint32_t *color_buffer;
    Color clear_color;
    int ven_width;
    int ven_height;
    int run;
    Figuras *figuras_buffer;
    // .figuras_buffer = 0;
    // .figuras_temp_buffer = 0;
}EstadosRender;

extern EstadosRender estadosrender;