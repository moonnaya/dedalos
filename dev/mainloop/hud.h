// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

// hud.h - Updates hud
update_time();

if (segundos == 0) {
    segundos = 4;
    sun_position++;
    if (sun_position > 7) {
        sun_position = 0;
    }
    print_time();
    previous_night_mode = night_mode;
    if (sun_position > 3) {
        night_mode = 1;
    } else {
        night_mode = 0;
    }
    // al pasar de dia a noche (o al reves) hay que repintar la pantalla
    if (previous_night_mode != night_mode) update_vision = 1;
}

