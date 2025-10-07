#pragma once
#include <SDL3/SDL.h>
#include "../draw/figuras.h"
#include "../color/colores.h"
#include "../global.h"

typedef enum {
    TOOL_CUADRO = 0,
    TOOL_TRIAN  = 1,
    TOOL_CIRC   = 2
} MiniTool;

void minipaint_init(void);
void minipaint_handle_event(const SDL_Event *ev);
void minipaint_draw_panel(void);
void minipaint_refill_all(void);

//  lee colores elegidos en la paleta
uint32_t minipaint_get_fill_color(void);
uint32_t minipaint_get_stroke_color(void);