// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

if (min_state) {                                          // 0: aun sin escudo
	if (min_state > 1) {                                  // saliste con el minotauro activo
		MIN_M.t = 0;
		if (min_hit || !min_retry [min_cur]) min_done |= (1 << min_cur);   // completada
		min_state = 1;
	}
	for (min_i = 0; min_i < MIN_NUM; min_i ++)
		if (min_pant [min_i] == n_pant && !(min_done & (1 << min_i))) {
			min_cur = min_i;
			min_state = 2; min_dir = 2; min_t = 0; min_hit = 0;
			MIN_M.x = min_x [min_cur] * 16;
			MIN_M.y = min_y [min_cur] * 16;
			MIN_M.t = 8;
			MIN_M.life = 255;
			break;
		}
}