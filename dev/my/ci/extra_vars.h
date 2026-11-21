// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins
// --- Aviso de esbirro cercano (cuadro de mensajes del marcador) ---
#define ESB_WARN_TILE		26		// tile de la cara. OJO: el 25 es la hogaza de pan
#define ESB_WARN_RADIUS		3		// radio en tiles (1 = solo las 8 casillas contiguas)
#define DED_TILE		44		// tile de la figura (OJO: choca con el 44 del mapa)
#define DED_PAGE_FRAMES	100
#define DED_FACE		28		// tile de la cara de Dedalo en el marcador

#define HUD_FACE_X		14		// tile X de la cara (el de mas a la derecha)
#define HUD_FACE_Y		11		// tile Y de la cara
#define HUD_MSG_X		20		// columna (en caracteres) del texto
#define HUD_MSG_Y		22		// fila de la primera linea (la segunda es 23)
#define HUD_MSG_ATTR	66		// atributo de las letras
#define HUNGER_SECONDS	10		// segundos por cada punto de hambre
unsigned char hunger_sec = 0;
unsigned char hud_ch[2] = {0, 0};
unsigned char hud_k;

unsigned char esb_warn = 0;
unsigned char esb_i, esb_t, esb_now, esb_px, esb_py, esb_dx, esb_dy, esb_r;

unsigned char action = 1;   //0 -> nada
                            //1 -> pan
                            //2 -> escudo
                            //3 -> hacha
unsigned char bread = 99;
unsigned char cuentafaps = 0;
unsigned char segundos = 0;
unsigned char prev_seg = 0;
unsigned char sun_position = 0;
unsigned char previous_night_mode = 0;
unsigned char hungry = 99;
unsigned char lifes = 3;
unsigned char eat = 0;

typedef struct {
    unsigned char pant;
    unsigned char pos;
} MARKED_TILE;

MARKED_TILE bread_tiles [100];
unsigned char door_pant[2] = { 20, 29 };
unsigned char door_pos[2]  = { 73, 58 };   // y*15 + x: (13,4) y (13,3)
unsigned int  door_i;                      // indice en mapa[]

typedef struct {
    unsigned char action;
    unsigned char used,in_floor;
    unsigned char tile;
    unsigned char old_tile;
    unsigned char pant;
    unsigned char x,y;
} OBJECT;

OBJECT o_null;
OBJECT o_bread;
OBJECT o_shield;
OBJECT o_axe;
OBJECT o_key_blue;
OBJECT o_key_yellow;

OBJECT objects[6];
unsigned char total_objects = 0;
int object_index = 0;

typedef struct {
    unsigned char pant;		// pantalla (255 = sin usar)
    unsigned char pos;		// y*15 + x dentro de la pantalla
    unsigned char visitado;
    unsigned char obj;		// indice en objects[]
    unsigned char *msg;		// paginas de 18 car. (9 + 9)
} DEDALO;

DEDALO ded[4];
unsigned char ded_cur = 0, ded_t = 0, ded_page = 0;
unsigned char ded_a, ded_b, ded_i;
