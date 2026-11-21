// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

if (min_state && min_state < 4) {
	if (n_pant == min_pant [min_cur]) {                      // armado: aparece
		min_state = 2; min_dir = 2; min_t = 0; min_hit = 0;
		MIN_M.x = min_x [min_cur] * 16;
		MIN_M.y = min_y [min_cur] * 16;
		MIN_M.t = 8;
		MIN_M.life = 255;
	} else if (min_state > 1) {                              // saliste con el minotauro activo
		MIN_M.t = 0;
		if (min_hit == 0 && min_retry [min_cur]) min_state = 1;   // vuelve a salir
		else {                                                    // aparicion completada
			min_cur ++;
			if (min_cur < MIN_NUM) min_state = 1; else min_state = 4;
		}
	}
}