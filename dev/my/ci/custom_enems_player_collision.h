// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

// To check custom collisions with the player.
// Runs after platforms (in lateral view games) have been discarded.

// Don't remember to bypass normal enemy/player collision (which results on the player losing life)
// if needed using 

// goto player_enem_collision_done;
if (_en_t == 8) {
	cx2 = _en_x; cy2 = _en_y;
	if (min_state == 3) goto player_enem_collision_done;     // aturdido: inofensivo
	if (action == 2 && min_front () && collide ()) {         // escudo equipado y de frente
		min_state = 3; min_hit = 1; min_t = min_stun [min_cur]; if (min_cur == 0) min_say = 1;
		for (min_i = 0; min_i < 48; min_i ++)                // retroceso brusco
			if (!min_step (min_dir ^ 1)) break;
		goto player_enem_collision_done;
	}
}