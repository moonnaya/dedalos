// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

// game_loop.h - Da game loop.

	#asm
		; Makes debugging easier
		._game_loop_init
	#endasm

	playing = 1;
	player_init ();

	


	#ifdef OBJECT_COUNT
		flags [OBJECT_COUNT] = p_objs;
	#endif



	#ifndef COMPRESSED_LEVELS		
		#if !defined DEACTIVATE_KEYS && MAX_CERROJOS > 0
			locks_init ();
		#endif
	#endif

	#if defined(PLAYER_STEPS_ON_ENEMIES) || defined (PLAYER_CAN_FIRE)
		#ifndef COMPRESSED_LEVELS
			enems_init ();
		#endif
	#endif	

	#ifndef COMPRESSED_LEVELS	
		n_pant = SCR_INICIO;
	#endif		

	maincounter = 0;

	#ifdef ACTIVATE_SCRIPTING		
		script_result = 0;
	#endif
	
	#ifdef MODE_128K
			// Play music
		#ifdef COMPRESSED_LEVELS		
			PLAY_MUSIC (levels [level].music_id);
		#else
			PLAY_MUSIC (1);
		#endif		
	#endif

	#ifdef ACTIVATE_SCRIPTING
		// Entering game
		run_script (2 * MAP_W * MAP_H);
	#endif

	#include "my/ci/entering_game.h"
	
	p_killme = success = half_life = 0;
		
	objs_old = keys_old = life_old = killed_old = 0xff;
	#ifdef MAX_AMMO 	
		ammo_old = 0xff;
	#endif
	#if defined(TIMER_ENABLE) && defined(PLAYER_SHOW_TIMER)
		timer_old = 0;
	#endif

	#ifdef PLAYER_CHECK_MAP_BOUNDARIES		
		#ifdef COMPRESSED_LEVELS
			x_pant = n_pant % level_data.map_w;
			y_pant = n_pant / level_data.map_w;
		#else
			x_pant = n_pant % MAP_W; y_pant = n_pant / MAP_W;
		#endif
	#endif

	#ifdef ENABLE_CHECKPOINTS
		if (sg_do_load) {
			mem_load ();
		}
	#endif		

	#ifdef MSC_MAXITEMS
		display_items ();
	#endif

	o_pant = 0xff;
	while (playing) {
		#asm
			; Makes debugging easier
			._game_loop_do
		#endasm

		#ifdef DEBUG_KEYS
			if (sp_KeyPressed (KEY_M)) { ++ p_objs; beep_fx (0); }
			if (sp_KeyPressed (KEY_H)) { ++ n_pant; beep_fx (0); }
			if (sp_KeyPressed (KEY_Y)) { -- n_pant; beep_fx (0); }
		#endif

		p_kill_amt = 1;
		pad0 = (joyfunc) (&keys);

		p_tx = (gpx + 8) >> 4;
		p_ty = (gpy + 8) >> 4;
	
		if (o_pant != n_pant) {
			#include "my/ci/before_entering_screen.h"
			update_vision = 1;
			draw_scr ();
			ded_solid ();
			update_vision = 0;
			o_pant = n_pant;
		}

		// if (previous_night_mode != night_mode) {
		// 	previous_night_mode = night_mode;
		// 	player_direction = previous_direction;
		// 	draw_scr_background();
		// 	update_vision_back();
		// 	invalidate_viewport ();
		// }

		if (update_vision == 1) {
			update_vision = 0;
			draw_scr_background();  // quizas se pueda mejorar esto, haciendo la operacion sin el calculo del mapa, solo cogiendo la pantalla ya cargada
			update_vision_back();
			invalidate_viewport ();
		}

		if (p_tx != p_txp || p_ty != p_typ) {
			update_vision_back();
		}

		if (first_time == 1) {
			print_lifes();
			first_time = 0;

			// bienvenida
			// _x = 14;
			// _y = 11;
			// _t = 28;
			// update_tile ();
			// _x = 19;
			// _y = 22;
			// _t = 5;
			// _gp_gen = "BIENVENIDO";
			// print_str ();
			// _x = 19;
			// _y = 23;
			// _t = 5;
			// _gp_gen = "JOVEN     ";
			// print_str ();
		}

		#include "mainloop/hud.h"

		maincounter ++;
		half_life = !half_life;

		p_tx = (gpx) >> 4;
		p_ty = (gpy) >> 4;
		p_txp = (p_x) >> 4;
		p_typ = (p_y) >> 4;
		
		// Move player
		player_move ();

		// Move enemies
		enems_move ();

		#ifdef ENABLE_SIMPLE_COCOS
			// Move simple cocos
			simple_coco_update ();
		#endif

		if (p_life > 1) {
			player_kill (p_killme);
			n_pant = safe_point.pant;
			gpx = safe_point.x << 4; p_x = gpx << FIXBITS;
			gpy = safe_point.y << 4; p_y = gpy << FIXBITS;
			p_tx = gpx >> 4;
			p_ty = gpy >> 4;
			player_direction = safe_point.direction;
			previous_direction = safe_point.direction;
			o_pant = 0xff;                 // recarga la pantalla y los enemigos
			update_vision = 1;
			print_lifes();
			#include "my/ci/on_player_killed.h"
		} else playing = 0;

		#ifdef PLAYER_CAN_FIRE
			// Move bullets 			
			bullets_move ();
		#endif

		// Detect fire zone
		#if defined ACTIVATE_SCRIPTING && defined ENABLE_FIRE_ZONE
			if (f_zone_ac == 1) {
				if (gpx >= fzx1 && gpx <= fzx2 && gpy >= fzy1 && gpy <= fzy2) {
					run_fire_script ();
				}	
			}
		#endif

		// Render
		if (o_pant == n_pant) {

			#include "mainloop/update_sprites.h"

			// Limit frame rate
			
			#ifdef MIN_FAPS_PER_FRAME
				#asm
					.ml_min_faps_loop
						ld  a, (_isrc)
						cp  MIN_FAPS_PER_FRAME
						jr  nc, ml_min_faps_loop_end
						halt
						jr  ml_min_faps_loop

					.ml_min_faps_loop_end
						xor a
						ld  (_isrc), a
				#endasm
			#endif

			sp_UpdateNow();
		}

		#ifdef PLAYER_FLICKERS
			// Flickering
			if (p_estado == EST_PARP) {
				p_ct_estado --;
				if (p_ct_estado == 0)
					p_estado = EST_NORMAL; 
			}
		#endif			
		
		// Hotspot interaction.


		// Scripting related stuff
		
		#ifdef ACTIVATE_SCRIPTING
			// Select object
			#ifdef MSC_MAXITEMS
				if (sp_KeyPressed (KEY_Z)) {
					if (!key_z_pressed) {
						#ifdef MODE_128K
							PLAY_SOUND (0);
						#else
							beep_fx (2);
						#endif
						flags [FLAG_SLOT_SELECTED] = (flags [FLAG_SLOT_SELECTED] + 1) % MSC_MAXITEMS;
						display_items ();
					}
					key_z_pressed = 1;
				} else {
					key_z_pressed = 0;
				}
			#endif			

			#ifndef SCRIPTING_KEY_NONE
				#ifdef SCRIPTING_KEY_M			
					if (sp_KeyPressed (KEY_M))
				#endif
				#ifdef SCRIPTING_DOWN
					if ((pad0 & sp_DOWN) == 0)
				#endif
				#ifdef SCRIPTING_KEY_FIRE
					if ((pad0 & sp_FIRE) == 0)
				#endif
				{
					// Any scripts to run in this screen?
					run_fire_script ();
				}
			#endif
		#endif

	

		// Flick the screen ?
			
		#if defined ACTIVATE_SCRIPTING && defined COMPRESSED_LEVELS
			if (script_result != 3)
		#endif
		{
			#include "mainloop/flick_screen.h"
		}

		// Win game condition
		
		if (0
			#ifdef ACTIVATE_SCRIPTING
				|| script_result == 1 || script_result > 2
			#endif
			#ifdef PLAYER_NUM_OBJETOS
				|| p_objs == PLAYER_NUM_OBJETOS
			#endif
			#ifdef SCR_FIN
				|| (n_pant == SCR_FIN
				#if defined PLAYER_FIN_X && defined PLAYER_FIN_Y				
					&& ((gpx + 8) >> 4) == PLAYER_FIN_X && ((gpy + 8) >> 4) == PLAYER_FIN_Y
				#endif
				)
			#endif
		) {
			success = 1;
			playing = 0;
		}
		
		// Game over condition
		#if defined ACTIVATE_SCRIPTING || (defined(TIMER_ENABLE) && defined(TIMER_GAMEOVER_0)) 
			if (0
			#ifdef ACTIVATE_SCRIPTING
				|| (script_result == 2)
			#endif
			#if defined(TIMER_ENABLE) && defined(TIMER_GAMEOVER_0)
				|| timer_zero
			#endif
		) {
			playing = 0;				
		}
		#endif

		#include "my/ci/extra_routines.h"
	}
	
	sp_UpdateNow ();
	sp_WaitForNoKey ();

	#ifdef MODE_128K		
		STOP_SOUND ();
	#endif

	#include "my/ci/after_game_loop.h"
