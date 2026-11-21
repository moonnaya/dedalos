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
	cuentafaps ++;
	if (cuentafaps == 25) {              // pasa un segundo
		cuentafaps = 0;
		segundos --;
		hunger_sec ++;
		if (hunger_sec >= HUNGER_SECONDS) {
			hunger_sec = 0;
			hungry --;
			if (lifes > 0) print_hungry();
		}
	}
	if (hungry == 0) {
		hungry = 50;
		print_hungry();
		p_killme = 4;                     // pasa por la misma muerte: baja p_life y vuelve al safe point
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
    for (rdb = 1; rdb <= 6; rdb++) {
        rdc = object_index + rdb;
        if (rdc >= 6) rdc -= 6;
        if (objects[rdc].action != 0 && objects[rdc].used == 0) {
            action = objects[rdc].action;
            object_index = rdc;
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

// Dibuja un tile en el marcador (filas 10-11 de tiles, fuera del mapa).
// No usa update_tile: con _y = 11 escribiria fuera de map_attr (150 bytes).
void hud_tile (unsigned char x, unsigned char y, unsigned char t) {
  _x = x; _y = y; _t = t;
  draw_invalidate_coloured_tile_gamearea ();
}

// Cuadro de mensajes del marcador: cara + 2 lineas de 9 caracteres.
// s = texto (los huecos que falten hasta 18 se rellenan con espacios),
// o 0 para borrar. face = tile de la cara, 0 = sin cara.
void hud_msg (unsigned char *s, unsigned char face) {
	hud_tile (HUD_FACE_X, HUD_FACE_Y, face);
	hud_ch[1] = 0;
	for (hud_k = 0; hud_k < 18; hud_k ++) {
		hud_ch[0] = 32;
		if (s && *s) { hud_ch[0] = *s; s ++; }
		_x = HUD_MSG_X + hud_k; _y = HUD_MSG_Y;
		if (hud_k >= 9) { _x -= 9; _y ++; }
		if (hud_ch[0] == 32) _t = 0; else _t = HUD_MSG_ATTR;
		_gp_gen = hud_ch;
		print_str ();
	}
}

void esb_update_warning (void) {
	if (ded_t) return;                  // no pisar el mensaje de Dedalo
	esb_now = esb_near ();
	if (esb_now != esb_warn) {          // solo se pinta al cambiar de estado
		esb_warn = esb_now;
		if (esb_warn) hud_msg ("         AAAAHRG!", ESB_WARN_TILE);  // 2a linea
		else hud_msg (0, 0);
	}
}

void ded_print_page(void) {
    hud_msg (ded[ded_cur].msg + ded_page * 18, DED_FACE);
}


// Hace solido a Dedalo (llamar justo despues de draw_scr ())
void ded_solid(void) {
    for (ded_i = 0; ded_i < 4; ded_i++)
        if (ded[ded_i].visitado == 0 && ded[ded_i].pant == n_pant)
            map_attr[ded[ded_i].pos] = 8;
}

void ded_solid(void) {
    for (ded_i = 0; ded_i < 4; ded_i++)
        if (ded_here (ded_i))
            map_attr[ded[ded_i].pos] = 8;
}

void ded_update(void) {
    if (ded_t) {                         // mensaje en curso
        ded_t--;
        if (ded_t == 0) {
            ded_page++;
            if (ded[ded_cur].msg[ded_page * 18] == 0) {
                hud_msg (0, 0);
                esb_warn = 0;            // que el aviso de esbirro pueda volver
            } else {
                ded_t = DED_PAGE_FRAMES;
                ded_print_page();
            }
        }
        return;
    }
    // posicion del jugador en tiles
    ded_a = (gpx + 8) >> 4;
    ded_b = (gpy + 8) >> 4;
    for (ded_cur = 0; ded_cur < 4; ded_cur++) {
        if (ded_here (ded_cur)) {
            // adyacente (incluye diagonales) a la casilla de Dedalo
            rdx = ded[ded_cur].pos % 15;
            rdy = ded[ded_cur].pos / 15;
            rda = ded_a + 1 - rdx;
            rdb = ded_b + 1 - rdy;
            if (rda <= 2 && rdb <= 2) {
                ded[ded_cur].visitado = 1;
                map_attr[ded[ded_cur].pos] = 0;
                // dar el objeto
                rdn = ded[ded_cur].obj;
                objects[rdn].used = 0;
                objects[rdn].in_floor = 0;
                if (objects[rdn].action == 3) p_ammo = 1;
                action = objects[rdn].action;   // equiparlo al recibirlo
                object_index = rdn;
                print_object();                 // pinta el icono en el marcador
                if (rdn == 4) maze_set (1);     // llave amarilla: se abre el laberinto
                // punto seguro
                safe_point.pant = n_pant;
                safe_point.x = ded_a;
                safe_point.y = ded_b;
                safe_point.direction = previous_direction;
                ded_page = 0;
                ded_t = DED_PAGE_FRAMES;
                ded_print_page();
                update_vision = 1;       // redibuja sin la figura
                return;
            }
        }
    }
    ded_cur = 0;
}

void door_check (void) {
	if (action < 4 || action > 5) return;
	rda = action - 4;                                  // 0 azul, 1 amarilla
	if (door_pant[rda] != n_pant) return;
	door_i = n_pant * 150 + door_pos[rda];
	if (mapa[door_i] == 7) return;                     // ya abierta
	rdx = (gpx + 8) >> 4; rdy = (gpy + 8) >> 4;        // tile del jugador
	rdb = door_pos[rda] % 15; rdc = door_pos[rda] / 15; // tile de la puerta
	if ((rdx == rdb && (rdy + 1 == rdc || rdy == rdc + 1)) ||
	    (rdy == rdc && (rdx + 1 == rdb || rdx == rdb + 1))) {
		mapa[door_i] = 7;                              // abierta para siempre
		_x = rdb; _y = rdc; _t = 7; _n = 0;
		update_tile ();
		mark_object_used ();                           // gasta la llave
		update_vision = 1;
	}
}

// open = 1: abre los pasos nuevos; open = 0: los vuelve a cerrar (pared = tile 1)
void maze_set (unsigned char open) {
	for (rda = 0; rda < 5; rda ++) {
		if (open) mapa[maze_pant[rda] * 150 + maze_pos[rda]] = maze_open[rda];
		else mapa[maze_pant[rda] * 150 + maze_pos[rda]] = 1;
	}
}