// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

if (min_state == 2 || min_state == 3) {                      // muerte: se rearma
	min_state = 1;
	malotes[MIN_PANT * MAX_ENEMS].t = 0;
}