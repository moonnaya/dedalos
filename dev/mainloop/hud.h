// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

// hud.h - Updates hud
update_time();

if (segundos == 0) {
    segundos = 60;
    sun_position++;
    if (sun_position > 7) {
        sun_position = 0;
    }
    print_time();
    if (sun_position > 3) {
        night_mode = 1;
    } else {
        night_mode = 0;
    }
}

