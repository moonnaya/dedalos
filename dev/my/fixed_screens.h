// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

#ifndef MODE_128K
	void lame_sound (void) {
		gpit = 4; do {
			beep_fx (rda);
			beep_fx (rdb);
		} while (-- gpit);
		beep_fx (3);
	}
#endif

void game_ending (void) {
	sp_UpdateNow();
	blackout ();
	#ifdef MODE_128K
		// Resource 2 = ending
		get_resource (ENDING_BIN, 16384);
	#else
		#asm
			ld hl, _s_ending
			ld de, 16384
			call depack
		#endasm
	#endif

	#ifdef MODE_128K
	#else
		rda = 7; rdb = 2;
		lame_sound ();
	#endif
	
	espera_activa (500);
}

void game_over (void) {
	sp_UpdateNow ();                 // vuelca lo pendiente antes de tocar la pantalla

	#asm
		ld  e, 7                     ; 7 pasos: el color mas alto (7) llega a 0
	.go_step
		ld  hl, 22528                ; area de atributos
		ld  bc, 768
	.go_cell
		ld  a, (hl)
		and 63                       ; quita FLASH y BRIGHT
		ld  d, a
		and 7                        ; tinta
		jr  z, go_ink
		dec d                        ; tinta - 1
	.go_ink
		ld  a, d
		and 56                       ; papel
		jr  z, go_paper
		ld  a, d
		sub 8
		ld  d, a                     ; papel - 1
	.go_paper
		ld  (hl), d
		inc hl
		dec bc
		ld  a, b
		or  c
		jr  nz, go_cell

		ld  bc, 8000                 ; pausa entre pasos (~60 ms)
	.go_wait
		dec bc
		ld  a, b
		or  c
		jr  nz, go_wait

		dec e
		jr  nz, go_step
	#endasm

	_x = 11; _y = 11; _t = 66; _gp_gen = (unsigned char *)("GAME"); print_str ();
	_x = 16; _y = 11; _t = 66; _gp_gen = (unsigned char *)("OVER"); print_str ();
	sp_UpdateNow ();

	#ifdef MODE_128K
	#else
		rda = 4; rdb = 0;
		lame_sound ();
	#endif

	espera_activa (500);
}

#if defined(TIMER_ENABLE) && defined(SHOW_TIMER_OVER)
	void time_over (void) {
		_x = 10; _y = 11; _t = 79; _gp_gen = spacer; print_str ();
		_x = 10; _y = 12; _t = 79; _gp_gen = (unsigned char *)(" TIME'S UP! "); print_str ();
		_x = 10; _y = 13; _t = 79; _gp_gen = spacer; print_str ();
		sp_UpdateNowEx (0);
			
		#ifdef MODE_128K
		#else
			rda = 1; rdb = 2;
		lame_sound ();
		#endif
		
		espera_activa (250);
	}
#endif


#ifdef COMPRESSED_LEVELS
	void zone_clear (void) {
		_x = 10; _y = 11; _t = 79; _gp_gen = spacer; print_str ();
		_x = 10; _y = 12; _t = 79; _gp_gen = (unsigned char *)(" ZONE CLEAR "); print_str ();
		_x = 10; _y = 13; _t = 79; _gp_gen = spacer; print_str ();
		sp_UpdateNowEx (0);
		espera_activa (250);			
	}
#endif
