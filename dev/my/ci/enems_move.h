// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins
case 8:
	_en_life = 255;                       // invulnerable
	active = 1;
	if (min_dir == 2) min_face = 0;       // izquierda; arriba/abajo conservan el ultimo lado
	else if (min_dir == 3) min_face = 1;
	if (min_state == 2) {
		min_move ();
		en_an_base_frame [enit] = 4 + (min_face << 1);       // 4-5 izquierda, 6-7 derecha
	} else {                                                  // aturdido: parpadea
		en_an_base_frame [enit] = 99;                         // el motor no anima: lo hacemos aqui
		if (maincounter & 8) en_an_next_frame [enit] = sprite_18_a;   // invisible
		else en_an_next_frame [enit] = enem_cells [4 + (min_face << 1)];
	}
	break;