// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins
// --- Aviso de esbirro cercano (cuadro de mensajes del marcador) ---
#define ESB_WARN_TILE		26		// tile de la cara. OJO: el 25 es la hogaza de pan
#define ESB_WARN_RADIUS		3		// radio en tiles (1 = solo las 8 casillas contiguas)
#define ESB_WARN_X			14		// tile X del marcador: 14 = el de mas a la derecha
#define ESB_WARN_Y			11		// tile Y: la fila de la seleccion de objetos
#define ESB_WARN_TXT_X		20		// columna (en caracteres) donde empieza el texto
#define ESB_WARN_TXT_Y		23		// fila (en caracteres) del texto
#define ESB_WARN_ATTR		66		// atributo del texto (el mismo que las vidas)

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
MARKED_TILE doors[2];

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
