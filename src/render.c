#include "render.h"
#include "color/colores.h"
#include "draw/draw.h"
#include "global.h"
#include "math/vectores.h"
#include "math/lerp.h"
#include "memoria/memoria.h"
#include <stdio.h>
#include "draw/figuras.h"

#include <stdlib.h>
#include <math.h>

Vec2* punto_seleccionado = NULL;

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
    Vec2 pc1 = {{400.f, 360.f}};
    Vec2 pc2 = {{600.f, 100.f}};
    Vec2 pc3 = {{800.f, 360.f}};

    Linea linea ={
        .p1 = (Vec2) {{800, 100}},
        .p2 = (Vec2) {{600, 250}},
        .offset_mem1 = {0},
        .color = (Color) {0xab00abff},
        .type = LINEA,
    };

    Curva curva ={
        .p1 = pc1,
        .p2 = pc2,
        .p3 = pc3,
        .color = (Color) {0xffff00ff},
        .type = CURVA
    };
    Circulo circ = {
        .pos = {{estadosrender.ven_width/2.f, estadosrender.ven_height/2.f}},
        .r = 20.f,
        .vert = 32,
        .offset_mem1 = {0},
        .color = (Color) {0x00ffffff},
        .type = CIRC
    };

    Cuadro cuadro = 
    {
        .pos = {{500.f,300.f}},
        .w = 100,
        .h = 50,
        .offset_mem1 = {0},
        .color = (Color) {0xff00ffff},
        .type = CUADRO
    };

    Figuras t = {*(FigComun*)&cuadro};
    Figuras test = {*(FigComun*)&linea};
    Figuras test2 = {.curva = curva};
    Figuras test3={*(FigComun*) &circ};

    pushtto_array(estadosrender.figuras_buffer, t);
    pushtto_array(estadosrender.figuras_buffer, test);
    pushtto_array(estadosrender.figuras_buffer, test2);
    pushtto_array(estadosrender.figuras_buffer, test3);

}

void update(void)
{
    // {
        
    //     Color Azul = {0x0000ffff};
        Color verde  = (Color){0x00ff00ff};
    // //     int y0=10;
    // //    for(int y=0; y<estadosrender.ven_height; y+=20){

    // //         draw_linea(0,y0,estadosrender.ven_width, y, Azul.hex);
    // //    }


    // // Color marron = (Color){0x8B4513ff};
    

    // // // Tronco: 40 px de ancho x 160 px de alto
    // // draw_cuadro(520, 520, 40, 160, marron.hex);

    // // draw_circulo(540, 460, 90,  verde.hex, 48);
    // // draw_circulo(515, 470, 70, verde.hex, 36);
    // // draw_circulo(565, 470, 70, verde.hex, 36);
    Vec2 p1 = {. unpack = {540, 380}};  
    Vec2 p2 = {. unpack = {510, 430}};  
    Vec2 p3 = {. unpack = {570, 430}};  
    Vec2 a = {. unpack = {670, 543}};  
    Vec2 b = {. unpack = {564, 657}};  
    Vec2 c = {. unpack = {230, 890}};  
    // triangulo de decoracion
    fill_triangulo(a,b,c,verde.hex);
    fill_triangulo(p1, p2, p3, verde.hex);



    //  Vec2 pc1 = {{400.f, 360.f}};
    // Vec2 pc2 = {{600.f, 100.f}};
    // Vec2 pc3 = {{800.f, 360.f}};
    
    // Linea linea ={
    //     .p1 = (Vec2){{800,100}},
    //     .p2 = (Vec2) {{600,250}},
    //     .offset_mem1 = {0},
    //     .color = (Color) {0xab00abff},
    //     .type = LINEA
    // };

    // Curva curva ={
    //     .p1 = pc1,
    //     .p2 = pc2,
    //     .p3 = pc3,
    //     .color = (Color) {0xffff00ff},
    //     .type = CURVA
    // };

    // Circulo circ = {
    //     .pos = {{estadosrender.ven_width/2.f, estadosrender.ven_height/2.f}},
    // };
    
    //     Vec2 P1 = { .unpack = {100, 400} };
    //     Vec2 P2 = { .unpack = {200, 100} };
    //     Vec2 P3 = { .unpack = {400, 600} };
    //     Vec2 P4 = { .unpack = {500, 300} };

    //     Vec2 A = { .unpack = {100, 200} };
    //     Vec2 B = { .unpack = {400, 350} };

       

    //     // Dibujar la curva
    //     draw_cubica(P1, P2, P3, P4, Azul.hex , 0.01f);
    //     draw_cuadratica(P1, P2, P3, Azul.hex, 0.01f);
    //     draw_lerp(A, B, Azul.hex, 0.01f);

        
    // }

// Carro
// draw_cuadro(200,500,100,40,0x0000FFFF);   // carrocería
// draw_cuadro(220,470,60,30,0x87CEEBFF);    // ventana
// draw_circulo(220,545,15,0x000000FF,20);   // llanta izq
// draw_circulo(280,545,15,0x000000FF,20);   // llanta der

    // draw_figura(&test1);
    // draw_figura(&test2);
    // draw_figura(&test3);

    for(int i=0; i<array_size(estadosrender.figuras_buffer); i++)
    {
        draw_figuras(&estadosrender.figuras_buffer[i]);
    }

}

void render_frame(void)
{
    SDL_RenderPresent(estadosrender.renderer);
}