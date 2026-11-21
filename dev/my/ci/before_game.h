// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins
// Solo el pan empieza en el inventario. used = 1 significa "no lo tiene";
// pasa a 0 al recogerlo (ver extra_routines.h)
objects[1].used = 1;   // escudo
objects[2].used = 1;   // hacha
objects[3].used = 1;   // llave azul
objects[4].used = 1;   // llave amarilla


// restart bread tiles
for (rda = 0; rda < 100; rda++) {
    bread_tiles[rda].pant = 255;
    bread_tiles[rda].pos = 0; 
}

// puertas cerradas: azul = tile 35 (indice 0), amarilla = tile 34 (indice 1)
for (rda = 0; rda < 2; rda++) map_set (door_pant[rda] * 150 + door_pos[rda], 35 - rda);

safe_point.pant = 0;
safe_point.x = 1;
safe_point.y = 1;
safe_point.direction = 0;

bread = 100;
hungry = 99;
segundos = 0;
o_null.action = 0;
o_bread.action = 1;
o_bread.used = o_bread.in_floor = 0;
o_shield.action = 2;
o_shield.used = o_shield.in_floor = 0;
o_shield.tile = 32;
o_axe.action = 3;
o_axe.used = o_axe.in_floor = 0;
o_axe.tile = 33;
o_key_blue.action = 4;
o_key_blue.used = o_key_blue.in_floor = 0;
o_key_blue.tile = 36;
o_key_yellow.action = 5;
o_key_yellow.used = o_key_yellow.in_floor = 0;
o_key_yellow.tile = 37;
for (rda = 0; rda < 6; rda++) {
    objects[rda] = o_null;
}
objects[0] = o_bread;
objects[1] = o_shield;
objects[2] = o_axe;
objects[3] = o_key_blue;
objects[4] = o_key_yellow;

objects[1].used = 1;   // escudo
objects[2].used = 1;   // hacha
objects[3].used = 1;   // llave azul
objects[4].used = 1;   // llave amarilla

total_objects = 5;
object_index = 0;

for (rda = 0; rda < 4; rda++) {
    ded[rda].pant = 255;
    ded[rda].visitado = 0;
    ded[rda].almas = 0;
}

ded[2].almas = 15;         // el escudo: hacen falta 15 almas liberadas

ded[0].pant = 12;
ded[0].pos = 28;
ded[0].obj = 2;
ded[0].msg = "EL HACHA LIBERA   LAS ALMASDE LOS   POSEIDOS. LIBERA 15 ALMAS          ";
ded[1].pant = 14;
ded[1].pos = 123;          
ded[1].obj = 3;            // llave azul
ded[1].msg = "LA LLAVE DE LA    BESTIA.  CUIDA TUSESPALDAS          ";
ded[2].pant = 9;
ded[2].pos = 118;          // y*15 + x = 7*15 + 13
ded[2].obj = 1;            // escudo (acción 2)
ded[2].msg = "EL ESCUDODETECTA  EL       CORAZON  DE LA    BESTIA Y LA       DETIENE  CUIDADO  VIENE PORTU       IZQUIERDA";
ded[3].pant = 5;
ded[3].pos = 54;           // y*15 + x = 3*15 + 9
ded[3].obj = 4;            // llave amarilla (acción 5)
ded[3].msg = "LA LLAVE DE LA    LIBERTAD Y MUERTE ";
ded_t = 0;
maze_set (0);                                    // laberinto cerrado
