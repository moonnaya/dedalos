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
#define MIN_PANT		15
#define MIN_X			14
#define MIN_Y			1
#define MIN_SPEED		4		// px por frame (el jugador va a 3)
#define MIN_STUN_SECS	10
// --- Minotauro: tabla de apariciones ---
#define MIN_NUM			3
#define MIN_SLOT		2		// hueco de enemigo (0-2) que usa; dejalo vacio en esas pantallas
#define MIN_M			malotes [min_pant [min_cur] * MAX_ENEMS + MIN_SLOT]
#define SOULS_NEEDED	1		// almas que hay que liberar
unsigned char souls_say = 0;
unsigned char min_pant[3]  = { 15, 4, 23 };
unsigned char min_x[3]     = { 14, 14, 13 };
unsigned char min_y[3]     = { 1, 1, 5 };
unsigned char min_spd[3]   = { 4, 2, 2 };		// px por frame (el jugador va a 3)
unsigned char min_stun[3]  = { 10, 5, 5 };		// segundos aturdido
unsigned char min_chase[3] = { 1, 0, 0 };		// 1 persigue, 0 patrulla
unsigned char min_retry[3] = { 0, 1, 1 };		// 1: si sales sin pararlo, vuelve a salir
unsigned char min_state = 0;	// 0 no activo, 1 armado, 2 activo, 3 aturdido, 4 terminadas
unsigned char min_cur = 0, min_hit = 0;
unsigned char min_dir = 2, min_t = 0, min_say = 0;	// 0 abajo, 1 arriba, 2 izq, 3 der
unsigned char min_i, min_d, min_c, min_nd, min_tx, min_ty, min_px, min_py, min_dist, min_best;
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
    unsigned char almas;	// almas que hay que liberar antes de que aparezca
} DEDALO;

DEDALO ded[6];
unsigned char ded_cur = 0, ded_t = 0, ded_page = 0;
unsigned char ded_a, ded_b, ded_i;

// El laberinto se abre al conseguir la llave amarilla (ultimo Dedalo)
unsigned char maze_pant[5] = { 23, 22, 17, 29, 29 };
unsigned char maze_pos[5]  = { 78, 127, 131, 18, 102 };   // y*15 + x: (3,5) (7,8) (11,8) (3,1) (12,6)
unsigned char maze_open[5] = { 7, 17, 7, 7, 7 };          // suelo que queda al abrir
