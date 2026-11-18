// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins
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
