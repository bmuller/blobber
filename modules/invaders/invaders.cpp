#include "invaders.h"

Alien::Alien(COORD _loc) { 
  reset(_loc);  
}

void Alien::reset(COORD _loc) {
  state = 0;
  speed = 1 + rand() % 10;
  loc.x = _loc.x;
  loc.y = _loc.y;
}

Explosion::Explosion(COORD _loc) {
  loc.x = _loc.x;
  loc.y = _loc.y;
  state = 0;
}

bool Alien::in_bounds(COORD c) { return( labs(c.x - loc.x - 40) < 40 && labs(c.y - loc.y - 40) < 40 ); }

Invaders::Invaders() : 
  paused(0x00), 
  level(5), 
  score(0), 
  ModInterface("Invaders") {} 

void Invaders::init(Camarea &area, ProjectionWindow &pw) {
  string s;

  // register some callbacks for Gtk events on the ProjectionWindow
  pw.signal_key_press_event().connect_notify(sigc::mem_fun(*this, &Invaders::key));
  pw.signal_expose_event().connect_notify(sigc::mem_fun(*this, &Invaders::expose));
  
  // read image files 
  pau = Gdk::Pixbuf::create_from_file(string(DATAROOTDIR) + "/invaders/pause.png");
  pau_blk = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, pau->get_bits_per_sample(), pau->get_width(), pau->get_height());
  pau_blk->fill(0x000000);
  ali = new Glib::RefPtr<Gdk::Pixbuf>[2];
  ali_blk = new Glib::RefPtr<Gdk::Pixbuf>[2];
  for(int i = 0; i < 2; i++) {
    int_to_string(i, s);
    ali[i] = Gdk::Pixbuf::create_from_file(string(DATAROOTDIR) + "/invaders/invader" + s + ".png");
    ali_blk[i] = Gdk::Pixbuf::create_from_file(string(DATAROOTDIR) + "/invaders/invaderblk" + s + ".png");    
  }
  kill = Gdk::Pixbuf::create_from_file(string(DATAROOTDIR) + "/invaders/kill.png");
  kill_blk = Gdk::Pixbuf::create_from_file(string(DATAROOTDIR) + "/invaders/killblk.png");
  
  // register poi
  register_poi(area, 1);

  // initialize aliens (5 in this case)
  for (int i = 0; i < 5; i++) {
    aliens.push_back(Alien(COORD(area.bounds.left + rand() % area.width, area.bounds.top)));
  }
}

Invaders::~Invaders() {
  delete [] ali;
  delete [] ali_blk;
}

void Invaders::update(Camarea &area, ProjectionWindow &pw) {
  COORD t;

  // return if the game is paused and "Paused" has been drawn
  if(paused == WAIT)  { return; }

  // get the Gdk::Window, recompute the scale
  Glib::RefPtr<Gdk::Window> window = pw.get_window();
  if(!window) { return; }

  // if the game was just paused, draw "Pause"
  if(paused == PAUSED) {
    paused = WAIT;
    pw.translate_coordinates( COORD( (area.width - pau->get_width() ) / 2 , (area.height - pau->get_height() ) / 2 ), t );
    window->draw_pixbuf(pw.get_style()->get_black_gc(),
                        pau, 0, 0, t.x, t.y, 
                        pau->get_width(),
                        pau->get_height(), 
                        Gdk::RGB_DITHER_NONE, 0, 0);
    return;
  } 
  if(paused == RESET) { 
    paused = NORMAL; 
    pw.translate_coordinates( COORD( (area.width - pau->get_width() ) / 2 , (area.height - pau->get_height() ) / 2 ), t );
    window->draw_pixbuf(pw.get_style()->get_black_gc(), 
                        pau_blk, 0, 0, t.x, t.y, 
                        pau->get_width(), 
                        pau->get_height(), 
                        Gdk::RGB_DITHER_NONE, 0, 0);
  }

  // get the poi
  vector<PIXEL> poi;
  get_poi(area, poi);
  
  if(poi.size() > 0) {
    cross.copy(poi[0].coord);
    //pw.translate_coordinates(cross,t);
    //window->draw_rectangle(pw.get_style()->get_white_gc(),true,t.x,t.y,5,5);
  }
  

  // if cross over ghost, blow up ghost, paint ghosts.
  for ( vector<Alien>::iterator it = aliens.begin(); it < aliens.end(); it++) {  
    pw.translate_coordinates(it->loc, t);
    window->draw_pixbuf(pw.get_style()->get_black_gc(), 
                        ali_blk[it->state], 0, 0, t.x, t.y, 
                        ali_blk[0]->get_width(), 
                        ali_blk[0]->get_height(), 
                        Gdk::RGB_DITHER_NONE, 0, 0);
    if ( it->in_bounds( cross ) ) { 
      kills.push_back(Explosion(it->loc));
      it->reset(COORD(area.bounds.left + rand() % (area.width-80), area.bounds.top));
      score++;
    }
    else if ( (it->loc.y += 5 * it->speed) > area.bounds.bottom - ali[0]->get_height() ) { 
      it->reset(COORD(area.bounds.left + rand() % (area.width-80), area.bounds.top));
    }
    pw.translate_coordinates(it->loc, t);
    window->draw_pixbuf(pw.get_style()->get_black_gc(), 
                        ali[(++(it->state)) %= 2], 0, 0, t.x, t.y, 
                        ali[0]->get_width(), 
                        ali[0]->get_height(), 
                        Gdk::RGB_DITHER_NONE, 0, 0);
  }

  // paint kills
  for ( vector<Explosion>::iterator it = kills.begin(); it < kills.end(); it++ ) { 
    pw.translate_coordinates(it->loc, t);    
    if( (it->state) == 0 ) { 
      window->draw_pixbuf(pw.get_style()->get_black_gc(), 
                          kill, 0, 0, t.x, t.y, 
                          kill_blk->get_width(), 
                          kill_blk->get_height(), 
                          Gdk::RGB_DITHER_NONE, 0, 0);
    }
    else if (it->state == 2) {
      window->draw_pixbuf(pw.get_style()->get_black_gc(), 
                          kill_blk, 0, 0, t.x, t.y, 
                          kill_blk->get_width(), 
                          kill_blk->get_height(), 
                          Gdk::RGB_DITHER_NONE, 0, 0);
      it = kills.erase(it);    
    }
    it->state++;
  }
}

void Invaders::key(GdkEventKey * event){
  switch(event->keyval) {
    case GDK_Pause : paused = paused^0x01; break;
    case GDK_r     : score = 0; break;
    case GDK_R     : score = 0; break;
    default        : break;
  }
}

void Invaders::expose(GdkEventExpose* event) {}

extern "C" {
  ModInterface *get_module() { return new Invaders(); };
};

