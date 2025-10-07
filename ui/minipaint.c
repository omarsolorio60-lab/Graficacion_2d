#include "minipaint.h"
#include "../draw/draw.h"
#include "../memoria/memoria.h"
#include "../global.h"
#include <SDL3/SDL.h>
#include <string.h>
#include <math.h>

// ======================== Estado de MiniPaint =========================
typedef struct {
    MiniTool tool;           // figura seleccionada en la barra
    Color    borde;          // color borde actual para nuevas figuras
    Color    relleno;        // color relleno actual para nuevas figuras
    int      modo_color;     // 0=edita borde, 1=edita relleno (cuando no hay Ctrl)
    int      panel_w;        // ancho del panel lateral
    int      hover_btn;      // -1 ninguno, 0-2 para botones de figura
    int      colocando;      // 1 si estamos capturando 3 clicks de triángulo
    int      clicks_nec;     // clicks que faltan para triángulo
    Vec2     puntos[3];      // clicks de triáagulo
    int      tamanio;        // tamaño base

    // seleccion
    int      sel_idx;        // indice de figura seleccionada (-1 = ninguna)
    int      arrastrando;    // 1 si estamos arrastrando
    int      drag_mx0, drag_my0; // mouse al iniciar drag
    int      base_x, base_y; // base para mover (segun tipo)
} MiniPaintState;
static MiniPaintState ui;

uint32_t minipaint_get_fill_color(void)   { return ui.relleno.hex; }
uint32_t minipaint_get_stroke_color(void) { return ui.borde.hex;   }



// color de relleno por-figura (paralelo a estadosrender.figuras_buffer)
static uint32_t *fills = NULL;

// paleta (RGBA)
static const uint32_t PALETA[] = {
    0x000000FF, 0xFFFFFFFF, 0xFF0000FF, 0x00FF00FF, 0x0000FFFF,
    0xFFFF00FF, 0xFF8800FF, 0x00FFFFFF, 0x8800FFFF, 0x808080FF
};
static const int NPALETA = (int)(sizeof(PALETA)/sizeof(PALETA[0]));

// ============================ Helpers UI ==============================
static int dentro(int mx,int my,int x,int y,int w,int h){
    return (mx>=x && mx<x+w && my>=y && my<y+h);
}

static void dibujar_cuadro_lleno(int x,int y,int w,int h,uint32_t col){
    draw_cuadro(x,y,w,h,col);
    for (int j=y+1;j<y+h-1;++j) draw_linea(x+1,j,x+w-2,j,col);
}

static void dibujar_boton(int x,int y,int w,int h,uint32_t col, int hover){
    dibujar_cuadro_lleno(x,y,w,h, col);
    if(hover) draw_cuadro(x+1,y+1,w-2,h-2,0x000000FF);
}

static void previsualizar_figura(int mx,int my){
    switch(ui.tool){
        case TOOL_CUADRO:
            draw_cuadro(mx - ui.tamanio/2, my - ui.tamanio/2, ui.tamanio, ui.tamanio, ui.borde.hex);
        break;
        case TOOL_CIRC:
            draw_circulo(mx, my, (float)ui.tamanio/2, ui.borde.hex, 32);
        break;
        case TOOL_TRIAN: {
            int s = ui.tamanio;
            Vec2 a = {{mx, my - s/2}};
            Vec2 b = {{mx - s/2, my + s/2}};
            Vec2 c = {{mx + s/2, my + s/2}};
            draw_triangulo(a.unpack.x,a.unpack.y,b.unpack.x,b.unpack.y,c.unpack.x,c.unpack.y, ui.borde.hex);
        } break;
    }
}

// ================= Hit-testing  ================
static int pt_in_rect(int mx,int my, const Figuras* f){
    int x=f->cuadro.pos.unpack.x, y=f->cuadro.pos.unpack.y;
    return (mx>=x && mx<x+f->cuadro.w && my>=y && my<y+f->cuadro.h);
}
static int pt_in_circ(int mx,int my, const Figuras* f){
    int cx=f->circulo.pos.unpack.x, cy=f->circulo.pos.unpack.y;
    int dx=mx-cx, dy=my-cy;
    return (dx*dx + dy*dy) <= (f->circulo.r * f->circulo.r);
}
static float area_sign(int x1,int y1,int x2,int y2,int x3,int y3){
    return (x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2))*0.5f;
}
static int pt_in_trian(int mx,int my, const Figuras* f){
    int ax=f->trirangulo.p1.unpack.x, ay=f->trirangulo.p1.unpack.y;
    int bx=f->trirangulo.p2.unpack.x, by=f->trirangulo.p2.unpack.y;
    int cx=f->trirangulo.p3.unpack.x, cy=f->trirangulo.p3.unpack.y;

    float a  = fabsf(area_sign(ax,ay,bx,by,cx,cy));
    float a1 = fabsf(area_sign(mx,my,bx,by,cx,cy));
    float a2 = fabsf(area_sign(ax,ay,mx,my,cx,cy));
    float a3 = fabsf(area_sign(ax,ay,bx,by,mx,my));
    // tolerancia pequeña
    return fabsf((a1+a2+a3) - a) < 0.5f;
}

static int hit_test(int mx,int my, const Figuras* f){
    switch (f->data.type){
        case CUADRO: return pt_in_rect(mx,my,f);
        case CIRC:   return pt_in_circ(mx,my,f);
        case TRIAN:  return pt_in_trian(mx,my,f);
        default:     return 0;
    }
}

static int pick_figure_top(int mx,int my){
    int n = array_size(estadosrender.figuras_buffer);
    for (int i=n-1; i>=0; --i){
        if (hit_test(mx,my,&estadosrender.figuras_buffer[i])) return i;
    }
    return -1;
}

// =================== Crear figuras + vector de fills ==================
static void colocar_figura_cuadro(int x,int y){
    Figuras f = (Figuras){0};
    f.data.type    = CUADRO;
    f.cuadro.pos   = (Vec2){{x - ui.tamanio/2, y - ui.tamanio/2}};
    f.cuadro.w     = ui.tamanio;
    f.cuadro.h     = ui.tamanio;
    f.cuadro.color = ui.borde;           // borde actual
    pushtto_array(estadosrender.figuras_buffer, f);
    ui.sel_idx = array_size(estadosrender.figuras_buffer)-1;

    pushtto_array(fills, ui.relleno.hex); // relleno que tenía la UI
    draw_figuras(&estadosrender.figuras_buffer[ui.sel_idx]);
    fill_figura(&estadosrender.figuras_buffer[ui.sel_idx], ui.relleno.hex, 1);
}

static void colocar_figura_circ(int x,int y){
    Figuras f = (Figuras){0};
    f.data.type     = CIRC;
    f.circulo.pos   = (Vec2){{x, y}};
    f.circulo.r     = ui.tamanio/2;
    f.circulo.vert  = 32;
    f.circulo.color = ui.borde;
    pushtto_array(estadosrender.figuras_buffer, f);
    ui.sel_idx = array_size(estadosrender.figuras_buffer)-1;

    pushtto_array(fills, ui.relleno.hex);
    draw_figuras(&estadosrender.figuras_buffer[ui.sel_idx]);
    fill_figura(&estadosrender.figuras_buffer[ui.sel_idx], ui.relleno.hex, 1);
}

static void colocar_figura_trian(Vec2 a, Vec2 b, Vec2 c){
    Figuras f = (Figuras){0};
    f.data.type            = TRIAN;
    f.trirangulo.p1        = a;
    f.trirangulo.p2        = b;
    f.trirangulo.p3        = c;
    f.trirangulo.Color.hex = ui.borde.hex; 
    pushtto_array(estadosrender.figuras_buffer, f);
    ui.sel_idx = array_size(estadosrender.figuras_buffer)-1;

    pushtto_array(fills, ui.relleno.hex);
    draw_figuras(&estadosrender.figuras_buffer[ui.sel_idx]);
    fill_figura(&estadosrender.figuras_buffer[ui.sel_idx], ui.relleno.hex, 1);
}

// Rellena todas leyendo su color guardado en 'fills'
void minipaint_refill_all(void){
    int n = array_size(estadosrender.figuras_buffer);
    int m = array_size(fills);
    int lim = (n < m) ? n : m;
    for(int i=0; i<lim; ++i){
        fill_figura(&estadosrender.figuras_buffer[i], fills[i], 1);
    }
}


void minipaint_init(void){
    memset(&ui, 0, sizeof(ui));
    ui.tool        = TOOL_CUADRO;
    ui.borde.hex   = 0x000000FF;  // negro
    ui.relleno.hex = 0xFF0000FF;  // rojo
    ui.modo_color  = 1;           // 0=borde, 1=relleno
    ui.panel_w     = 140;
    ui.hover_btn   = -1;
    ui.colocando   = 0;
    ui.clicks_nec  = 0;
    ui.tamanio     = 80;

    ui.sel_idx     = -1;
    ui.arrastrando = 0;
    ui.drag_mx0 = ui.drag_my0 = 0;
    ui.base_x = ui.base_y = 0;
}

void minipaint_handle_event(const SDL_Event *ev){
    // Hover basico en los botones
    if(ev->type == SDL_EVENT_MOUSE_MOTION){
        int mx = ev->motion.x, my = ev->motion.y;
        ui.hover_btn = -1;
        if(dentro(mx,my, 10,10, 120,30)) ui.hover_btn = 0;
        else if(dentro(mx,my, 10,50,120,30)) ui.hover_btn = 1;
        else if(dentro(mx,my, 10,90,120,30)) ui.hover_btn = 2;

        // Si estamos arrastrando, mueve la figura seleccionada
        if (ui.arrastrando && ui.sel_idx >= 0) {
            int dx = mx - ui.drag_mx0;
            int dy = my - ui.drag_my0;
            Figuras *f = &estadosrender.figuras_buffer[ui.sel_idx];
            switch (f->data.type){
                case CUADRO:
                    f->cuadro.pos.unpack.x = ui.base_x + dx;
                    f->cuadro.pos.unpack.y = ui.base_y + dy;
                break;
                case CIRC:
                    f->circulo.pos.unpack.x = ui.base_x + dx;
                    f->circulo.pos.unpack.y = ui.base_y + dy;
                break;
                case TRIAN: {
                    int cx = ui.base_x, cy = ui.base_y; // centro base
                    int ncx = cx + dx, ncy = cy + dy;
                    // delta a aplicar a cada punto
                    int ddx = ncx - cx, ddy = ncy - cy;
                    f->trirangulo.p1.unpack.x += ddx; f->trirangulo.p1.unpack.y += ddy;
                    f->trirangulo.p2.unpack.x += ddx; f->trirangulo.p2.unpack.y += ddy;
                    f->trirangulo.p3.unpack.x += ddx; f->trirangulo.p3.unpack.y += ddy;
                    // actualiza la base para no acumular deltas dobles
                    ui.base_x = ncx; ui.base_y = ncy;
                    ui.drag_mx0 = mx; ui.drag_my0 = my;
                } break;
                default: break;
            }
        }
        return;
    }

    if(ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
        int mx = (int)ev->button.x;
        int my = (int)ev->button.y;
        

        // click dentro del panel lateral?
        const int px = 0, py = 0, pw = ui.panel_w, ph = estadosrender.ven_height;
        if(dentro(mx,my, px,py,pw,ph)){
            // cambiar herramienta
            if(dentro(mx,my, 10,10,120,30)) ui.tool = TOOL_CUADRO;
            else if(dentro(mx,my, 10,50,120,30)) ui.tool = TOOL_TRIAN;
            else if(dentro(mx,my, 10,90,120,30)) ui.tool = TOOL_CIRC;

            // cambiar modo color
            if(dentro(mx,my, 10,140, 50,20)) ui.modo_color = 0;
            if(dentro(mx,my, 80,140, 50,20)) ui.modo_color = 1;

            // paleta: dos filas de 5
            int startY = 170;
            for(int i=0;i<NPALETA;i++){
                int cx = 10 + (i%5)*26;
                int cy = startY + (i/5)*26;
                if(dentro(mx,my, cx,cy, 22,22)){
                    uint32_t chosen = PALETA[i];

                    // con CTRL: aplica a figura seleccionada si hay
                    if ((SDL_GetModState() & SDL_KMOD_CTRL) && ui.sel_idx >= 0) {
                        Figuras *f = &estadosrender.figuras_buffer[ui.sel_idx];
                        if (ev->button.button == SDL_BUTTON_RIGHT) {
                            // cambiar BORDE de la figura seleccionada
                            switch (f->data.type){
                                case CUADRO: f->cuadro.color.hex = chosen; break;
                                case CIRC:   f->circulo.color.hex = chosen; break;
                                case TRIAN:  f->trirangulo.Color.hex = chosen; break;
                                default: break;
                            }
                        } else {
                            // cambiar RELLENO de la figura seleccionada
                            if (ui.sel_idx < array_size(fills)) fills[ui.sel_idx] = chosen;
                        }
                    } else {
                        // sin CTRL: cambia colores "actuales" (para figuras nuevas)
                        if (ev->button.button == SDL_BUTTON_RIGHT) ui.borde.hex   = chosen;
                        else                                        ui.relleno.hex = chosen;
                    }
                }
            }
            return;
        }

        // click en lienzo: intentar seleccionar/arrastrar si botón izq
        if (ev->button.button == SDL_BUTTON_LEFT) {
            int idx = pick_figure_top(mx,my);
            if (idx >= 0) {
                ui.sel_idx = idx;
                ui.arrastrando = 1;
                ui.drag_mx0 = mx; ui.drag_my0 = my;
                Figuras *f = &estadosrender.figuras_buffer[idx];
                switch (f->data.type){
                    case CUADRO:
                        ui.base_x = f->cuadro.pos.unpack.x;
                        ui.base_y = f->cuadro.pos.unpack.y;
                    break;
                    case CIRC:
                        ui.base_x = f->circulo.pos.unpack.x;
                        ui.base_y = f->circulo.pos.unpack.y;
                    break;
                    case TRIAN: {
                        // centro aproximado como base
                        int cx = (f->trirangulo.p1.unpack.x + f->trirangulo.p2.unpack.x + f->trirangulo.p3.unpack.x)/3;
                        int cy = (f->trirangulo.p1.unpack.y + f->trirangulo.p2.unpack.y + f->trirangulo.p3.unpack.y)/3;
                        ui.base_x = cx; ui.base_y = cy;
                    } break;
                    default: break;
                }
                return;
            }
        }

        // si no selecciono, insertar figura nueva
        if(ui.tool == TOOL_CUADRO){
            colocar_figura_cuadro(mx,my);
        } else if(ui.tool == TOOL_CIRC){
            colocar_figura_circ(mx,my);
        } else if(ui.tool == TOOL_TRIAN){
            if(!ui.colocando){ ui.colocando=1; ui.clicks_nec=3; }
            int i = 3 - ui.clicks_nec;
            ui.puntos[i] = (Vec2){{mx,my}};
            ui.clicks_nec--;
            if(ui.clicks_nec==0){
                colocar_figura_trian(ui.puntos[0], ui.puntos[1], ui.puntos[2]);
                ui.colocando=0;
            }
        }
        return;
    }

    if (ev->type == SDL_EVENT_MOUSE_BUTTON_UP){
        ui.arrastrando = 0;
        return;
    }

    if (ev->type == SDL_EVENT_MOUSE_WHEEL){
        ui.tamanio += (ev->wheel.y>0 ? 5 : -5);
        if(ui.tamanio < 10) ui.tamanio = 10;
        if(ui.tamanio > 400) ui.tamanio = 400;
        return;
    }

    if (ev->type == SDL_EVENT_KEY_DOWN){
        SDL_Scancode sc = ev->key.scancode;
        if (sc == SDL_SCANCODE_ESCAPE) { ui.sel_idx = -1; ui.arrastrando = 0; }

        if (sc == SDL_SCANCODE_EQUALS || sc == SDL_SCANCODE_KP_PLUS) {
            ui.tamanio += 5; if (ui.tamanio > 400) ui.tamanio = 400;
        }
        if (sc == SDL_SCANCODE_MINUS || sc == SDL_SCANCODE_KP_MINUS) {
            ui.tamanio -= 5; if (ui.tamanio < 10) ui.tamanio = 10;
        }
        return;
    }
}

void minipaint_draw_panel(void){
    // Panel de la izquierda
    dibujar_cuadro_lleno(0,0, ui.panel_w, estadosrender.ven_height, 0x333333FF);

    // Botones de figura
    dibujar_boton(10,10,120,30, (ui.tool==TOOL_CUADRO?0x8888FFFF:0x999999FF), ui.hover_btn==0);
    dibujar_boton(10,50,120,30, (ui.tool==TOOL_TRIAN ?0x8888FFFF:0x999999FF), ui.hover_btn==1);
    dibujar_boton(10,90,120,30, (ui.tool==TOOL_CIRC  ?0x8888FFFF:0x999999FF), ui.hover_btn==2);

    // Iconitos
    draw_cuadro(20,20,15,15,0x000000FF);
    draw_triangulo(20,70, 35,95, 50,70, 0x000000FF);
    draw_circulo(30,105,10,0x000000FF,24);

    // Selector de modo color
    dibujar_boton(10,140,50,20, ui.modo_color==0?0xBBBBBBFF:0x777777FF,0); // Borde
    dibujar_boton(80,140,50,20, ui.modo_color==1?0xBBBBBBFF:0x777777FF,0); // Relleno

    // Paleta (dos filas de 5)
    int startY = 170;
    for(int i=0;i<NPALETA;i++){
        int cx = 10 + (i%5)*26;
        int cy = startY + (i/5)*26;
        dibujar_cuadro_lleno(cx,cy, 22,22, PALETA[i]);
    }

    // Prev solo si el mouse esta en el lienzo
    float mx_f, my_f; SDL_GetMouseState(&mx_f, &my_f);
    int mx = (int)mx_f, my = (int)my_f;
    if (mx >= ui.panel_w && mx < estadosrender.ven_width && my >= 0 && my < estadosrender.ven_height) {
        previsualizar_figura(mx, my);
    }
}