// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

if (n_pant == MIN_PANT) {
	if (min_state >= 1 && min_state <= 3) {                  // armado o en curso: aparece
		min_state = 2; min_dir = 2; min_t = 0;
		malotes[MIN_PANT * MAX_ENEMS].x = MIN_X * 16;
		malotes[MIN_PANT * MAX_ENEMS].y = MIN_Y * 16;
		malotes[MIN_PANT * MAX_ENEMS].t = 8;
		malotes[MIN_PANT * MAX_ENEMS].life = 255;
	}
} else if (min_state == 2 || min_state == 3) {               // saliste de la pantalla 15
	min_state = 4;
	malotes[MIN_PANT * MAX_ENEMS].t = 0;
}