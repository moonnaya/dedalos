// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins
void print_number_wan (void) {
  rda = 16 + (index / 10); rdb = 16 + (index % 10);
  #asm
      ; enter:  A = row position (0..23)
      ;         C = col position (0..31/63)
      ;         D = pallette #
      ;         E = graphic #

      ld  a, (_rda)
      ld  e, a

      ld  a, (__n)
      ld  d, a
      
      ld  a, (__x)
      ld  c, a

      ld  a, (__y)

      call SPPrintAtInv

      ld  a, (_rdb)
      ld  e, a

      ld  a, (__n)
      ld  d, a
      
      ld  a, (__x)
      inc a
      ld  c, a

      ld  a, (__y)

      call SPPrintAtInv
  #endasm
}

void print_hungry() {
      _x = 11;
      _y = 23;
      index = hungry;
      if (hungry > 50) _n = 4;
      else if (hungry > 25) _n = 3;
      else _n = 2;
      print_number_wan();
}

void update_time(void) {
	cuentafaps ++; if (cuentafaps == 25 && segundos >= 0) { cuentafaps = 0; segundos --; }
  if (segundos != prev_seg) {
    prev_seg = segundos;
    if (segundos % 10 == 0) {
        hungry--;
        if (lifes > 0) print_hungry();
    }
  }
  if (hungry == 0) {
      lifes--;
      //indicar mensaje
      //retornar a punto de salvado.
      hungry = 50;
      print_lifes();
      if (lifes > 0) print_hungry();
  }
}

void print_lifes() {
  rdb = p_life;
  for (rda = 0; rda < 3; rda++) {
      _x = 12 - rda;
      _y = 22;
      if (rdb > 0) {
          _gp_gen = "_";
          _t = 66;
          rdb--;
      } else {
        _t = 0;
        _gp_gen = " ";
      }
      print_str ();
  }
}

void print_object() {
    if (objects[object_index].action == 1) {
      print_bread();
    } else if (objects[object_index].action != 0) {
        hud_tile (7, 11, objects[object_index].tile);
    }
}

void next_object() {
  if (object_index + 1 >= 5) {
        object_index = -1;
    }
    for (rdb = object_index + 1; rdb < 6; rdb++) {
        if (objects[rdb].action != 0 && objects[rdb].used == 0) {
            action = objects[rdb].action;
            object_index = rdb;
            print_object();
            break;
        }
    }
}

void mark_object_used() {
    for (rda = 0; rda < 6; rda++) {
        if (objects[rda].action == action) {
            objects[rda].used = 1;
            next_object();
            break;
        }
    }
}

// Tiles (izquierdo y derecho) del sol/luna para cada sun_position (0..7)
unsigned char sun_left  [8] = { 19, 20,  0, 24, 22, 23,  0, 21 };
unsigned char sun_right [8] = {  0, 21, 19, 20,  0, 24, 22, 23 };

void print_time(void) {
  if (sun_position < 8) {
    hud_tile (0, 11, sun_left  [sun_position]);
    hud_tile (1, 11, sun_right [sun_position]);
  }
}

void print_bread() {
  if (bread > 75) {
    _t = 25;
  } else if (bread > 50) {
    _t = 29;
  } else if (bread > 25) {
    _t = 30;
  } else if (bread > 0) {
    _t = 31;
  } else {
    _t = 15;
  }
  hud_tile (7, 11, _t);
}

// 1 si hay algun esbirro vivo dentro del radio de aviso
unsigned char esb_near (void) {
	esb_px = (gpx + 8) >> 4;
	esb_py = (gpy + 8) >> 4;
	// con el aviso puesto se exige un tile mas para quitarlo: evita parpadeo
	esb_r = ESB_WARN_RADIUS + esb_warn;

	for (esb_i = 0; esb_i < MAX_ENEMS; esb_i ++) {
		esb_t = malotes [enoffs + esb_i].t;
		// tipo 0 = hueco vacio; bit 4 = enemigo muerto
		if ((esb_t & 0x0f) == 0 || (esb_t & 0x10)) continue;

		esb_dx = (malotes [enoffs + esb_i].x + 8) >> 4;
		if (esb_dx >= esb_px) esb_dx -= esb_px; else esb_dx = esb_px - esb_dx;
		esb_dy = (malotes [enoffs + esb_i].y + 8) >> 4;
		if (esb_dy >= esb_py) esb_dy -= esb_py; else esb_dy = esb_py - esb_dy;

		if (esb_dx <= esb_r && esb_dy <= esb_r) return 1;
	}
	return 0;
}

void esb_draw_warning (unsigned char on) {
	// La cara. No uso update_tile: con _y = 11 escribe fuera de map_attr/map_buff
	_x = ESB_WARN_X; _y = ESB_WARN_Y;
	if (on) _t = ESB_WARN_TILE; else _t = 0;
	draw_invalidate_coloured_tile_gamearea ();

	// El texto, a su izquierda
	_x = ESB_WARN_TXT_X; _y = ESB_WARN_TXT_Y; _t = ESB_WARN_ATTR;
	if (on) _gp_gen = "Aaaahrg"; else _gp_gen = "       ";
	print_str ();
}

void esb_update_warning (void) {
	esb_now = esb_near ();
	if (esb_now != esb_warn) {          // solo se pinta al cambiar de estado
		esb_warn = esb_now;
		esb_draw_warning (esb_warn);
	}
}

// Dibuja un tile en el marcador (filas 10-11 de tiles, fuera del mapa).
// No usa update_tile: con _y = 11 calcula el indice 15*11 + x = 165..179 y
// escribe fuera de map_attr (150 bytes), pisando otras variables.
void hud_tile (unsigned char x, unsigned char y, unsigned char t) {
  _x = x; _y = y; _t = t;
  draw_invalidate_coloured_tile_gamearea ();
}