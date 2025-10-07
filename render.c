#include "render.h"
#include "color/colores.h"
#include "draw/draw.h"
#include "global.h"
#include "math/vectores.h"
#include "math/lerp.h"
#include "memoria/memoria.h"
#include <stdio.h>
#include "draw/figuras.h"
#include "ui/minipaint.h"

#include <stdlib.h>
#include <math.h>

Vec2* punto_seleccionado = NULL;
extern uint32_t ui_fill_color;
extern uint32_t ui_stroke_color;


Vec2 *pivote_mas_cerca(Vec2 mp, Figuras* figs, float umbral)
{

    int a = 0;
    int b = array_size(figs)-1;
    float high;
    float low;

    float centro;

    for(int i=0; i<array_size(figs); i++)
    {
        int r = (a+b) / 2;
        low = distanciav2(mp,figs[a].cuadro.pos);
        high = distanciav2(mp,figs[b].cuadro.pos);
        centro = distanciav2(mp,figs[r].cuadro.pos);
        if(abs(low) <= umbral)
            return &figs[a].cuadro.pos;
        else if (abs(high) <= umbral)
            return &figs[b].cuadro.pos;
       else if(fabs(centro)<= umbral)
            return &figs[r].cuadro.pos;
            
        if(r<b)
            b=4;
        else if(r>a)
            a=r;

    }
    return NULL;
}
void render_input(void)
{
     if(estadosrender.evento.type == SDL_EVENT_QUIT)
    {
        estadosrender.run = 0;
    }
    if(estadosrender.evento.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        //buscar el pivote mas cerca del mouse
        printf("(%.3f, %.3f)\n", estadosrender.evento.motion.x,estadosrender.evento.motion.y);
        //
        if(!punto_seleccionado)
        {
            punto_seleccionado = pivote_mas_cerca(

            (Vec2){{estadosrender.evento.motion.x,
            
            estadosrender.evento.motion.y}},
            estadosrender.figuras_buffer,
            10.f
            );
        if(punto_seleccionado)
        {
            printf("Punto cercano(%.3f, %.3f)\n", punto_seleccionado->unpack.x,punto_seleccionado->unpack.y);
        }
        }
       
        //obtener la referencia de la figura
        //
        //modificar
    }
    else if(estadosrender.evento.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        if(punto_seleccionado)
        {
            punto_seleccionado->unpack.x = estadosrender.evento.motion.x;
            punto_seleccionado->unpack.y = estadosrender.evento.motion.y;
            punto_seleccionado = NULL;
        }
    }
}

void clear_color_buffer(void)
{
    for(int y=0;y<estadosrender.ven_height;++y)
    {
        for(int x=0;x<estadosrender.ven_width;++x)
        {
            draw_pixel(x,y,estadosrender.clear_color.hex);
        }
    }
}

void copy_buffer_to_texture(void)
{
    SDL_UpdateTexture(estadosrender.textura,
            NULL,
            estadosrender.color_buffer,
           (int)( estadosrender.ven_width*sizeof(int))
           );
    SDL_RenderTexture(estadosrender.renderer,estadosrender.textura,NULL,NULL);
}

void _init(void)
{
    //inicializzar las variables a usar
    // Vec2 pc1 = {{400.f, 360.f}};
    // Vec2 pc2 = {{600.f, 100.f}};
    // Vec2 pc3 = {{800.f, 360.f}};

    // Linea linea ={
    //     .p1 = (Vec2) {{800, 100}},
    //     .p2 = (Vec2) {{600, 250}},
    //     .offset_mem1 = {0},
    //     .color = (Color) {0xab00abff},
    //     .type = LINEA,
    // };

    // Curva curva ={
    //     .p1 = pc1,
    //     .p2 = pc2,
    //     .p3 = pc3,
    //     .color = (Color) {0xffff00ff},
    //     .type = CURVA
    // };
    Circulo circ = {
        .pos = {{estadosrender.ven_width/2.f, estadosrender.ven_height/2.f}},
        .r = 20.f,
        .vert = 32,
        .offset_mem1 = {0},
        .color = (Color) { ui_stroke_color }, // borde
        .fill  = (Color) { ui_fill_color   }, // relleno
        .type = CIRC
    };

    Cuadro cuadro =
    {
        .pos = {{500.f,300.f}},
        .w = 100,
        .h = 50,
        .offset_mem1 = {0},
        .color = (Color) { ui_stroke_color }, // borde
        .fill  = (Color) { ui_fill_color   }, // relleno
        .type = CUADRO
    };

    Figuras t     = {.cuadro  = cuadro};
    // Figuras test  = {.linea   = linea};
    // Figuras test2 = {.curva   = curva};
    Figuras test3 = {.circulo = circ};

    pushtto_array(estadosrender.figuras_buffer, t);
    // pushtto_array(estadosrender.figuras_buffer, test);
    // pushtto_array(estadosrender.figuras_buffer, test2);
    pushtto_array(estadosrender.figuras_buffer, test3);
  

}

void update(void)
{
    



    // Dibujar todas las figuras (ahora ya rellenan dentro de draw_figuras)
    int n = array_size(estadosrender.figuras_buffer);
    for(int i=0;i<n;i++){
        draw_figuras(&estadosrender.figuras_buffer[i]);
    }

    // Panel lateral
    minipaint_draw_panel();

   

}

void render_frame(void)
{
    SDL_RenderPresent(estadosrender.renderer);
}