// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins
void print_number_wan (void) {
  rda = 16 + (index / 10); rdb = 16 + (index % 10);
  #asm
      ; enter:  A = row position (0..23)
      ;         C = col position (0..31/63)
      ;         D = pallette #
      ;         E = graphic #

      ld  a, (_rda)
      ld  e, a

      ld  a, (__n)
      ld  d, a
      
      ld  a, (__x)
      ld  c, a

      ld  a, (__y)

      call SPPrintAtInv

      ld  a, (_rdb)
      ld  e, a

      ld  a, (__n)
      ld  d, a
      
      ld  a, (__x)
      inc a
      ld  c, a

      ld  a, (__y)

      call SPPrintAtInv
  #endasm
}

void print_hungry() {
      _x = 11;
      _y = 23;
      index = hungry;
      if (hungry > 50) _n = 4;
      else if (hungry > 25) _n = 3;
      else _n = 2;
      print_number_wan();
}

void update_time(void) {
	cuentafaps ++; if (cuentafaps == 25 && segundos >= 0) { cuentafaps = 0; segundos --; }
  if (segundos != prev_seg) {
    prev_seg = segundos;
    if (segundos % 10 == 0) {
        hungry--;
        if (lifes > 0) print_hungry();
    }
  }
  if (hungry == 0) {
      lifes--;
      //indicar mensaje
      //retornar a punto de salvado.
      hungry = 50;
      print_lifes();
      if (lifes > 0) print_hungry();
  }
}

void print_lifes() {
  rdb = p_life;
  for (rda = 0; rda < 3; rda++) {
      _x = 12 - rda;
      _y = 22;
      if (rdb > 0) {
          _gp_gen = "_";
          _t = 66;
          rdb--;
      } else {
        _t = 0;
        _gp_gen = " ";
      }
      print_str ();
  }
}

void print_object() {
    if (objects[object_index].action == 1) {
      print_bread();
    } else if (objects[object_index].action != 0) {
        _x = 7;
        _y = 11;
        _t = objects[object_index].tile;
        update_tile(); 
    }
}

void next_object() {
  if (object_index + 1 >= 5) {
        object_index = -1;
    }
    for (rdb = object_index + 1; rdb < 6; rdb++) {
        if (objects[rdb].action != 0 && objects[rdb].used == 0) {
            action = objects[rdb].action;
            object_index = rdb;
            print_object();
            break;
        }
    }
}

void mark_object_used() {
    for (rda = 0; rda < 6; rda++) {
        if (objects[rda].action == action) {
            objects[rda].used = 1;
            next_object();
            break;
        }
    }
}

void print_time(void) {
  if (sun_position == 0) {
    _x = 0;
    _y = 11;
    _t = 19;
    update_tile();
    _x = 1;
    _y = 11;
    _t = 0;
    update_tile();
  }
  if (sun_position == 1) {
    _x = 0;
    _y = 11;
    _t = 20;
    update_tile();
    _x = 1;
    _y = 11;
    _t = 21;
    update_tile();
  }
  if (sun_position == 2) {
    _x = 0;
    _y = 11;
    _t = 0;
    update_tile();
    _x = 1;
    _y = 11;
    _t = 19;
    update_tile();
  }
  if (sun_position == 3) {
    _x = 0;
    _y = 11;
    _t = 24;
    update_tile();
    _x = 1;
    _y = 11;
    _t = 20;
    update_tile();
  }
  if (sun_position == 4) {
    _x = 0;
    _y = 11;
    _t = 22;
    update_tile();
    _x = 1;
    _y = 11;
    _t = 0;
    update_tile();
  }
  if (sun_position == 5) {
    _x = 0;
    _y = 11;
    _t = 23;
    update_tile();
    _x = 1;
    _y = 11;
    _t = 24;
    update_tile();
  }
  if (sun_position == 6) {
    _x = 0;
    _y = 11;
    _t = 0;
    update_tile();
    _x = 1;
    _y = 11;
    _t = 22;
    update_tile();
  }
  if (sun_position == 7) {
    _x = 0;
    _y = 11;
    _t = 21;
    update_tile();
    _x = 1;
    _y = 11;
    _t = 23;
    update_tile();
  }
}

void print_bread() {
  _x = 7;
  _y = 11;
  if (bread > 75) {
    _t = 25;
  } else if (bread > 50) {
    _t = 29;
  } else if (bread > 25) {
    _t = 30;
  } else if (bread > 0) {
    _t = 31;
  } else {
    _t = 15;
  }
  update_tile();
}