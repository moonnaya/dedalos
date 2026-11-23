// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

eat = 0;
esb_update_warning ();
ded_update ();
gpjt = sp_GetKey ();
if (gpjt == 'w' && total_objects > 1) {  //migas
    next_object();
} else if (gpjt == 'e') {
    eat = 1;
}
// else if (gpjt == '1') {
//     night_mode = 1;
// }

rdt = qtile(p_tx, p_ty);

for (rdn = 0; rdn < 6; rdn++) {
    if (objects[rdn].tile != 0 && rdt == objects[rdn].tile) {
        objects[rdn].used = 0;
        objects[rdn].in_floor = 0;
        _x = objects[rdn].x;
        _y = objects[rdn].y;
        _t = objects[rdn].old_tile;
        _n = behs [_t];
        update_tile();
        if (objects[rdn].action == 3) {
            p_ammo = 1;
        }
        action = objects[rdn].action;     // equiparlo al recogerlo
        object_index = rdn;
        print_object();                   // pinta su icono en el marcador
        update_vision = 1;
    }
}

rdn = pad0 & sp_FIRE;

// dejar migas
if (action == 1 && bread > 0) {
    if (rdn == 0 && eat == 0) {
        _t = qtile(p_tx, p_ty);
        if (_t != 15 && _t != 14) {  
            bread--;
            _x = p_tx;
            _y = p_ty;
            if (night_mode == 0) _t = 15; else _t = 14;
            _n = 0;
            update_tile ();
            map_buff [p_tx + (p_ty << 4) - p_ty] = 15;
            bread_tiles[99 - bread].pant = n_pant;
            bread_tiles[99 - bread].pos = p_tx + (p_ty * 15);
            print_bread();
        }
    } else if (eat == 1) {
        eat = 0;
        // bread es unsigned: restar 10 con menos de 10 daba la vuelta (~250)
        // y luego bread_tiles[99 - bread] escribia fuera del array.
        if (bread >= 10) rdt = 10; else rdt = bread;
        bread = bread - rdt;
        hungry = hungry + rdt;
        if (hungry > 99) hungry = 99;
        print_hungry();
        print_bread();
    }
    if (bread == 0) {
        objects[0].used = 1;
        action = 0;                  // nada equipado...
        hud_tile (7, 11, 0);         // ...icono vacio...
        next_object ();              // ...y pasa al siguiente objeto, si hay alguno
    }
}

door_check ();
