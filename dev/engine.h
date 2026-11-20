// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins
unsigned char first_time = 1;
#ifdef TEST_DEBUG
	void test_debug (void) {
		sp_UpdateNow ();
		sp_PrintAtInv (5, 5, 71, 69);
		sp_PrintAtInv (6, 6, 15, 70);
		#asm
				ld  ix, (_sp_player)
				ld  iy, vpClipStruct

				ld  bc, 0

				ld  hl, 0x0707 
				ld  de, 0x0000

				call SPMoveSprAbs
		#endasm
		sp_UpdateNow ();
		espera_activa (50);
		#asm
				ld  ix, (_sp_player)
				ld  iy, vpClipStruct

				ld  bc, 0

				ld  hl, 0x0404 
				ld  de, 0x0000

				call SPMoveSprAbs
		#endasm
		sp_UpdateNow ();
		while (sp_GetKey ());
		while (!sp_GetKey ());
	}
#endif

// Motor.h
#ifndef PLAYER_MIN_KILLABLE
#define PLAYER_MIN_KILLABLE 0
#endif

#if defined PLAYER_GENITAL || defined PLAYER_CUSTOM_ANIMATION
	// right: 0 + frame
	// left: 2 + frame
	// up: 4 + frame
	// down: 6 + frame
	const unsigned char *player_cells [] = {
		sprite_1_a, sprite_2_a, sprite_3_a, sprite_4_a,
		sprite_5_a, sprite_6_a, sprite_7_a, sprite_8_a
	};
#elif defined PLAYER_BOOTEE
	// vy = 0: 0 + facing
	// vy < 0: 1 + facing
	// vy > 0: 2 + facing
	const unsigned char *player_cells [] = {
		sprite_5_a, sprite_6_a, sprite_7_a, sprite_8_a,
		sprite_1_a, sprite_2_a, sprite_3_a, sprite_4_a
	};
#else
	// Normal animation:
	// 0 1 2 3 + facing: walk, 1 = stand. 8 + facing = jump/fall
	const unsigned char *player_cells [] = {
		sprite_5_a, sprite_6_a, sprite_7_a, sprite_6_a,
		sprite_1_a, sprite_2_a, sprite_3_a, sprite_2_a,
		sprite_8_a, sprite_4_a
	};
#endif

const unsigned char *enem_cells [] = {
	sprite_9_a, sprite_10_a, sprite_11_a, sprite_12_a, 
	sprite_13_a, sprite_14_a, sprite_15_a, sprite_16_a
};

#include "my/fixed_screens.h"

signed int addsign (signed int n, signed int value) {
	if (n >= 0) return value; else return -value;
}

#ifdef ENABLE_TILANIMS
	#include "tilanim.h"
#endif

void espera_activa (int espera) {
	while (sp_GetKey ());
	do {
		#ifndef MODE_128K
			gpjt = 250; do { gpit = 1; } while (--gpjt);
		#else
			#asm
				halt
			#endasm
		#endif
		if (sp_GetKey ()) break;
	} while (--espera);
}

#ifndef COMPRESSED_LEVELS
	#if !defined DEACTIVATE_KEYS && MAX_CERROJOS > 0
		void locks_init (void) {
			for (gpit = 0; gpit < MAX_CERROJOS; ++ gpit) cerrojos [gpit].st = 1;	
		}
	#endif
#endif



#ifdef ACTIVATE_SCRIPTING
	void run_fire_script (void) {
		run_script (2 * MAP_W * MAP_H + 2); 		// PRESS_FIRE AT ANY
		run_script ((n_pant << 1) + 1); 			// PRESS_FIRE AT SCREEN n
	}
#endif

#if defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS)
	void process_tile (void) {
		#ifdef PLAYER_PUSH_BOXES
			#ifdef FIRE_TO_PUSH
				if ((pad0 & sp_FIRE) == 0)				
			#endif		
			{ 
				#ifdef FIRE_TO_PUSH			
					// Para no disparar...
					//pushed_any = 1;
					p_disparando = 1;
				#endif

				if (qtile (x0, y0) == 14 && attr (x1, y1) == 0 && x1 < 15 && y1 < 10) {
					rda = map_buff [COORDS(x1,y1)];
					
					#if defined(ACTIVATE_SCRIPTING) && defined(ENABLE_PUSHED_SCRIPTING)
						flags [MOVED_TILE_FLAG] = rda; 
						flags [MOVED_X_FLAG] = x1;
						flags [MOVED_Y_FLAG] = y1;
					#endif			
					
					// Pintar
					_x = x0; _y = y0; _t = 0; _n = 0; update_tile ();
					_x = x1; _y = y1; _t = 14; _n = 10; update_tile ();
					
					// Sonido
					#ifdef MODE_128K
						PLAY_SOUND (SFX_PUSH_BOX);
					#else			
						beep_fx (2);	
					#endif
				
					#if defined(ACTIVATE_SCRIPTING) && defined(ENABLE_PUSHED_SCRIPTING) && defined(PUSHING_ACTION)
						// Call scripting
						just_pushed = 1;
						run_fire_script ();
						just_pushed = 0;
					#endif

					#include "my/ci/on_tile_pushed.h"
				} 
			}			
		#endif

		#if !defined DEACTIVATE_KEYS && MAX_CERROJOS > 0
			if (qtile (x0, y0) == 15 && p_keys) {
				for (gpit = 0; gpit < MAX_CERROJOS; ++ gpit) {
					if (cerrojos [gpit].x == x0 && cerrojos [gpit].y == y0 && cerrojos [gpit].np == n_pant) {
						cerrojos [gpit].st = 0;
						break;
					}
				}
				_x = x0; _y = y0; _t = 0; _n = 0; update_tile ();
				-- p_keys;
		
				#ifdef MODE_128K
					PLAY_SOUND (SFX_OPEN_LOCK);
				#else
					beep_fx (8);
				#endif

				#include "my/ci/on_unlocked_bolt.h"
			}
		#endif
	}
#endif

void update_vision_back() {
	
	// if (player_direction == 0) { // abajo
	// 	if (p_ty > 1) {
	// 		if (p_tx > 0) {
	// 			_x = p_tx - 1;
	// 			_y = p_ty - 2;
	// 			_t = 0;
	// 			_n = 8;
	// 			update_tile ();
	// 		}
	// 		_x = p_tx;
	// 		_y = p_ty - 2;
	// 		_t = 0;
	// 		_n = 8;
	// 		update_tile ();
	// 		if (p_tx < 14) {
	// 			_x = p_tx + 1;
	// 			_y = p_ty - 2;
	// 			_t = 0;
	// 			_n = 8;
	// 			update_tile ();
	// 		}
			
	// 	}
	// } else if (player_direction == 1) { //arriba
	// 	if (p_ty < 8) {
	// 		if (p_tx > 0) {
	// 			_x = p_tx - 1;
	// 			_y = p_ty + 2;
	// 			_t = 0;
	// 			_n = 8;
	// 			update_tile ();
	// 		}
	// 		_x = p_tx;
	// 		_y = p_ty + 2;
	// 		_t = 0;
	// 		_n = 8;
	// 		update_tile ();
	// 		if (p_tx < 14) {
	// 			_x = p_tx + 1;
	// 			_y = p_ty + 2;
	// 			_t = 0;
	// 			_n = 8;
	// 			update_tile ();
	// 		}
			
	// 	}
		
	// } else if (player_direction == 2) { //izq
	// 	if (p_tx < 13) {
	// 		if (p_ty > 0) {
	// 			_x = p_tx + 2;
	// 			_y = p_ty - 1;
	// 			_t = 0;
	// 			_n = 8;
	// 			update_tile ();
	// 		}
	// 		_x = p_tx + 2;
	// 		_y = p_ty;
	// 		_t = 0;
	// 		_n = 8;
	// 		update_tile ();
	// 		if (p_ty < 9) {
	// 			_x = p_tx + 2;
	// 			_y = p_ty + 1;
	// 			_t = 0;
	// 			_n = 8;
	// 			update_tile ();
	// 		}
			
	// 	}
	// } else if (player_direction == 3) { //der
	// 	if (p_tx > 1) {
	// 		if (p_ty > 0) {
	// 			_x = p_tx - 2;
	// 			_y = p_ty - 1;
	// 			_t = 0;
	// 			_n = 8;
	// 			update_tile ();
	// 		}
	// 		_x = p_tx - 2;
	// 		_y = p_ty;
	// 		_t = 0;
	// 		_n = 8;
	// 		update_tile ();
	// 		if (p_ty < 9) {
	// 			_x = p_tx - 2;
	// 			_y = p_ty + 1;
	// 			_t = 0;
	// 			_n = 8;
	// 			update_tile ();
	// 		}
			
	// 	}
		
	// }

}

unsigned char isWall(unsigned char tile) {
	if (behs[tile] == 8) return 1; else return 0;
}

/*
void fill_screen_axis_y() {
	rdb = (rda * 15) + p_tx;
	i = rdb + (150 * n_pant);
	screen_flat[rdb] = mapa[i];
	if (rdb - 1 >= 0) {
		screen_flat[rdb - 1] = mapa[i - 1];
	}
	if (rdb + 1 < 150) {
		screen_flat[rdb + 1] = mapa[i + 1];
	}

}*/

 /*
void fill_screen_axis_x() {
	rdb = p_ty * 15;
	i = rdb + rda + (n_pant * 150);
	rdc = rda + rdb;
	screen_flat[rdc] = mapa[i];
	if (rdc - 15 >= 0) {
		screen_flat[rdc - 15] = mapa[i - 15];
	}
	if (rdc + 15 < 150) {
		screen_flat[rdc + 15] = mapa[i + 15];
	}
}

*/

void fill_screen_axis_y() {
	rdb = (rda * 15) + p_tx;
	i = rdb + (150 * n_pant);
	screen_flat[rdb] = mapa[i];
	// Se comprueba la columna, no rdb: si no, en los bordes se cuela la fila vecina
	if (p_tx > 0) screen_flat[rdb - 1] = mapa[i - 1];
	if (p_tx < 14) screen_flat[rdb + 1] = mapa[i + 1];
}

void fill_screen_axis_x() {
	rdb = p_ty * 15;
	i = rdb + rda + (n_pant * 150);
	rdc = rda + rdb;
	screen_flat[rdc] = mapa[i];
	if (p_ty > 0) screen_flat[rdc - 15] = mapa[i - 15];
	if (p_ty < 9) screen_flat[rdc + 15] = mapa[i + 15];
}

// 1 si la posicion (x + y * 15) cae dentro de la zona que ve el jugador
unsigned char pos_in_vision(unsigned char pos) {
	rdx = pos % 15;
	rdy = pos / 15;
	if (rdx < xini || rdx > xfin || rdy < yini || rdy > yfin) return 0;
	return 1;
}

void vision() {
	update_vision = 0;

	for (rda = 0; rda < 150; rda++) {
		screen_flat[rda] = 0;
	}

	// player_direction vale 4 con el jugador parado (ver player.h), asi que la
	// vista se calcula con previous_direction, que guarda hacia donde mira.
	// rda es unsigned char: "rda >= 0" siempre es cierto, por eso los bucles
	// hacia arriba/izquierda salen con un break explicito al llegar a 0.

	if (previous_direction == 0) { // abajo
		if (p_ty > 0) { rda = p_ty - 1; fill_screen_axis_y(); }
		for (rda = p_ty; rda < 10; rda++) {
			fill_screen_axis_y();
			if (isWall(screen_flat[rdb])) break;
		}
		if (p_ty > 0) yini = p_ty - 1; else yini = 0;
		if (rda > 9) yfin = 9; else yfin = rda;
		if (p_tx > 0) xini = p_tx - 1; else xini = 0;
		if (p_tx < 14) xfin = p_tx + 1; else xfin = 14;
	} else if (previous_direction == 1) { // arriba
		if (p_ty < 9) { rda = p_ty + 1; fill_screen_axis_y(); }
		rda = p_ty;
		while (1) {
			fill_screen_axis_y();
			if (isWall(screen_flat[rdb])) break;
			if (rda == 0) break;
			rda--;
		}
		yini = rda;
		if (p_ty < 9) yfin = p_ty + 1; else yfin = 9;
		if (p_tx > 0) xini = p_tx - 1; else xini = 0;
		if (p_tx < 14) xfin = p_tx + 1; else xfin = 14;
	} else if (previous_direction == 2) { // izquierda
		rda = p_tx;
		while (1) {
			fill_screen_axis_x();
			if (isWall(screen_flat[rdc])) break;
			if (rda == 0) break;
			rda--;
		}
		xini = rda;
		xfin = p_tx;
		if (p_ty > 0) yini = p_ty - 1; else yini = 0;
		if (p_ty < 9) yfin = p_ty + 1; else yfin = 9;
	} else if (previous_direction == 3) { // derecha
		for (rda = p_tx; rda < 15; rda++) {
			fill_screen_axis_x();
			if (isWall(screen_flat[rdc])) break;
		}
		xini = p_tx;
		if (rda > 14) xfin = 14; else xfin = rda;
		if (p_ty > 0) yini = p_ty - 1; else yini = 0;
		if (p_ty < 9) yfin = p_ty + 1; else yfin = 9;
	}

	// Restaurar lo dejado en el suelo, solo dentro de la linea de vision
	for (gpit = 0; gpit < 100; gpit++) {
		if (bread_tiles[gpit].pant == n_pant && pos_in_vision(bread_tiles[gpit].pos)) {
			if (night_mode == 0) screen_flat[bread_tiles[gpit].pos] = 15;
			else screen_flat[bread_tiles[gpit].pos] = 14;
		}
	}

	for (gpit = 0; gpit < 2; gpit++) {
		if (doors[gpit].pant == n_pant && pos_in_vision(doors[gpit].pos)) {
			screen_flat[doors[gpit].pos] = 7;
		}
	}

	for (gpit = 0; gpit < 6; gpit++) {
		if (objects[gpit].in_floor == 1 && objects[gpit].pant == n_pant) {
			rdn = objects[gpit].x + (objects[gpit].y * 15);
			if (pos_in_vision(rdn)) screen_flat[rdn] = objects[gpit].tile;
		}
	}

	for (gpit = 0; gpit < 4; gpit++) {
		if (ded[gpit].visitado == 0 && ded[gpit].pant == n_pant && pos_in_vision(ded[gpit].pos))
			screen_flat[ded[gpit].pos] = DED_TILE;
	}

	// Version nocturna de los tiles (1..6 -> 8..13, 7 -> 16, 15 -> 14, 17 -> 18)
	if (night_mode == 1) {
		for (rda = 0; rda < 150; rda++) {
			rdt = screen_flat[rda];
			if (rdt >= 1 && rdt <= 6) screen_flat[rda] = rdt + 7;
			else if (rdt == 7) screen_flat[rda] = 16;
			else if (rdt == 15) screen_flat[rda] = 14;
			else if (rdt == 17) screen_flat[rda] = 18;
		}
	}
}

void draw_screen() {
	map_pointer = screen_flat;
	
	#ifdef USE_MAP_CUSTOM_DECODER
		#include "my/map_custom_decoder.h"
	#else
		for (gpit = 0; gpit < 150; ++ gpit) {	
			
			// Mapa tipo PACKED
			_t = *map_pointer ++;
			
			map_attr [gpit] = behs [_t];
			//if (_t == 0 && (rand () & 15) == 1) _t = 19;
			map_buff [gpit] = _t;
			/*
			#asm
					ld  a, (_gpit)
					and 1
					jr  nz, _draw_scr_packed_existing
				._draw_scr_packed_new
					ld  hl, (_map_pointer)
					ld  a, (hl)
					ld  (_gpc), a
					inc hl
					ld  (_map_pointer), hl

					srl a
					srl a
					srl a
					srl a
					jr  _draw_scr_packed_done

				._draw_scr_packed_existing
					ld  a, (_gpc)
					and 15

				._draw_scr_packed_done
					ld  (__t), a
					
					ld  b, 0
					ld  c, a
					ld  hl, _behs
					add hl, bc
					ld  a, (hl)

					ld  bc, (_gpit)
					ld  b, 0
					ld  hl, _map_attr
					add hl, bc
					ld  (hl), a
			
			#ifdef PACKED_MAP_ALT_TILE
					ld  a, (__t)
					or  a
					jr  nz, _draw_scr_packed_noalt

				._draw_scr_packed_alt
					call _rand
					ld  a, l
					and 15
					cp  1
					jr  z, _draw_scr_packed_alt_subst

					ld  a, (__t)
					jr  _draw_scr_packed_noalt

				._draw_scr_packed_alt_subst
					ld  a, PACKED_MAP_ALT_TILE
					ld  (__t), a

				._draw_scr_packed_noalt
			#endif

					ld  hl, _map_buff
					add hl, bc
					
					ld  (hl), a
			#endasm
			*/
			

			draw_coloured_tile ();

			//_x += 2; if (_x == VIEWPORT_X + 30) { _x = VIEWPORT_X; _y += 2; }
			
			#asm
					ld  a, (__x)
					add 2
					cp  30 + VIEWPORT_X
					jr  c, _advance_worm_no_inc_y

					ld  a, (__y)
					add 2
					ld  (__y), a

					ld  a, VIEWPORT_X

				._advance_worm_no_inc_y
					ld  (__x), a
			#endasm 
		}
	#endif


}


void draw_scr_background (void) {
	seed = n_pant;
	_x = VIEWPORT_X; _y = VIEWPORT_Y;
	vision();
	draw_screen();
}


void draw_scr (void) {
	is_rendering = 1;

	#ifdef ENABLE_FIRE_ZONE
		f_zone_ac = 0;
	#endif	

	draw_scr_background ();

	// Object setup

	enems_load ();

	#ifdef ACTIVATE_SCRIPTING
		#if defined LINE_OF_TEXT && !defined LINE_OF_TEXT_NO_AUTOERASE
			_x = LINE_OF_TEXT_X; _y = LINE_OF_TEXT; _t = LINE_OF_TEXT_ATTR; _gp_gen = "                              "; print_str ();
		#endif
		// Ejecutamos los scripts de entrar en pantalla:
		run_script (2 * MAP_W * MAP_H + 1); 	// ENTERING ANY
		run_script (n_pant << 1); 				// ENTERING SCREEN n
	#endif

	#include "my/ci/entering_screen.h"

	#ifdef PLAYER_CAN_FIRE
		bullets_init ();
	#endif

	#ifdef ENABLE_SIMPLE_COCOS
		simple_coco_init ();
	#endif

	invalidate_viewport ();
	is_rendering = 0;
}

void select_joyfunc (void) {
	#ifdef MODE_128K
	#else
		#asm
			; Music generated by beepola
			call musicstart
		#endasm
	#endif
	
	while (1) {
		gpjt = sp_GetKey ();
		if (gpjt >= '1' && gpjt <= '3') {
			joyfunc = joyfuncs [gpjt - '1'];

			#ifdef USE_TWO_BUTTONS
				isJoy = gpjt > '1';
			#endif

			break;
		}			
	}

	#ifdef MODE_128K
		STOP_SOUND ();
		PLAY_SOUND (SFX_START);
		sp_WaitForNoKey ();
	#endif
}

#ifdef WALLS_STOP_ENEMIES
	unsigned char mons_col_sc_x (void) {
		cx1 = cx2 = (_en_mx > 0 ? _en_x + 15 : _en_x) >> 4;
		cy1 = _en_y >> 4; cy2 = (_en_y + 15) >> 4;
		cm_two_points ();
		#ifdef EVERYTHING_IS_A_WALL
			return (at1 || at2);
		#else
			return ((at1 & 8) || (at2 & 8));
		#endif
	}
		
	unsigned char mons_col_sc_y (void) {
		cy1 = cy2 = (_en_my > 0 ? _en_y + 15 : _en_y) >> 4;
		cx1 = _en_x >> 4; cx2 = (_en_x + 15) >> 4;
		cm_two_points ();
		#ifdef EVERYTHING_IS_A_WALL
			return (at1 || at2);
		#else
			return ((at1 & 8) || (at2 & 8));
		#endif
	}
#endif


