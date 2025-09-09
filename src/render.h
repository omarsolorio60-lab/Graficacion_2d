#pragma once
/*
Es el archivo interfaz oara mostrar en pantalla
*/
#include <stdint.h>
#include "draw/figuras.h"
void clear_color_buffer();
void update(void);
void _init(void);
void copy_buffer_to_texture(void);
void render_frame(void);
void render_input(void);
Vec2 *pivote_mas_cerca(Vec2 mp, Figuras* figs, float umbral);