#include "invaders.h"

Invaders::Invaders() : 
  paused(0x00), 
  level(5), 
  score(0), 
  ModInterface("Invaders") {} 

void Invaders::init(Camarea &area, ProjectionWindow &pw) {
  string s, red, green, blue;

  // register some callbacks for Gtk events on the ProjectionWindow
  pw.signal_key_press_event().connect_notify(sigc::mem_fun(*this, &Invaders::key));
  pw.signal_expose_event().connect_notify(sigc::mem_fun(*this, &Invaders::expose));
  
  // read image files 
  pau = Gdk::Pixbuf::create_from_file(string(DATAROOTDIR) + "/invaders/pause.png");
  cro = Gdk::Pixbuf::create_from_file(string(DATAROOTDIR) + "/invaders/cross.png");
  gho = new Glib::RefPtr<Gdk::Pixbuf>[9];
  bam = new Glib::RefPtr<Gdk::Pixbuf>[8];
  for(int i = 0; i < 9; i++) {
    int_to_string(i+1, s);
    gho[i] = Gdk::Pixbuf::create_from_file(string(DATAROOTDIR) + "/invaders/ghost" + s + ".png");    
  }
  for(int i = 0; i < 8; i++) {
    int_to_string(i+1, s);
    bam[i] = Gdk::Pixbuf::create_from_file(string(DATAROOTDIR) + "/invaders/explode" + s + ".png");
  }

  // set the scale factor between the camarea and pw
  pw.get_window()->get_size(win_width, win_height);
  scale[0] = (double) win_width / (double) area.width;
  scale[1] = (double) win_height / (double) area.height;
  resize();

  // register poi
  config_get_set("red", red, "60");
  config_get_set("blue", blue, "0");
  config_get_set("green", green, "0");
  CRANGE range(COLOR(string_to_int(red), string_to_int(blue), string_to_int(green)));
  register_poi_criteria(area, range, 1);

  // start the timer
  //timer = clock();
 
  cross.x = area.bounds.left + 100;
  cross.y = area.bounds.top + 100;
 
  // initialize aliens (ghosts?)
  aliens_size = 5;
  aliens = (Entity *) malloc( aliens_size * sizeof(Entity) );
  for (int i = 0; i < aliens_size; i++) {
    aliens[i].loc.x = area.bounds.left + rand() % (area.width - 60);
    aliens[i].loc.y = area.bounds.top;
    aliens[i].speed = 1 + rand() % (level+ 1);
    aliens[i].state = 0;
  }
}

Invaders::~Invaders() {
  delete [] gho;
  delete [] bam;
}

void Invaders::update(Camarea &area, ProjectionWindow &pw) {
  int dest_x, dest_y, dest_w, dest_h;

  // the clock is not currently used
  //clock_t split_timer = clock();
  //float elapsed_time = ( (float) split_timer - timer) / CLOCKS_PER_SEC;
  //timer = split_timer;

  // return if the game is paused and "Paused" has been drawn
  if(paused == WAIT)  { return; }

  // get the Gdk::Window, recompute the scale
  Glib::RefPtr<Gdk::Window> window = pw.get_window();
  if(!window) { return; }
  scale[0] = (double) win_width / (double) area.width;
  scale[1] = (double) win_height / (double) area.height;

  // if the game was just paused, draw "Pause"
  if(paused == PAUSED) {
    paused = WAIT;
    dest_x = floor( ( win_width - scale[0]*pau->get_width() ) / 2.0 );
    dest_y = floor( ( win_height - scale[1]*pau->get_height() ) / 2.0 );
    dest_w = ceil(scale[0]*pau->get_width());
    dest_h = ceil(scale[1]*pau->get_height());
    pau->composite(scr, dest_x, dest_y, dest_w, dest_h, dest_x, dest_y, scale[0], scale[1], Gdk::INTERP_BILINEAR, 255);
    scr->render_to_drawable_alpha(window, 0, 0, 0, 0, win_width, win_height, Gdk::PIXBUF_ALPHA_BILEVEL, 255, Gdk::RGB_DITHER_NORMAL, 0, 0);
    return;
  } 
  if(paused == RESET) { paused = NORMAL; }

  // get the poi
  vector<PIXEL> poi;
  get_poi(area, poi);
   
  if ( poi.size() != 0 ) {
    if( poi[0].coord.x > area.bounds.right - 10 ) {poi[0].coord.x = area.bounds.right - 10;}
    else if( poi[0].coord.x < area.bounds.left + 10 ) {poi[0].coord.x = area.bounds.left + 10;}
    if( poi[0].coord.y > area.bounds.bottom - 20 ) {poi[0].coord.y = area.bounds.bottom - 20;}
    else if( poi[0].coord.y < area.bounds.top + 20 ) {poi[0].coord.y = area.bounds.top + 20;}
    cross.copy(poi[0].coord);
  }
  
  // start with black opaque background
  scr->fill(0x000000FF);  

  // paint the 'cross'hairs
  dest_x = floor( scale[0]*(cross.x - 8) );
  dest_y = floor( scale[1]*(cross.y - 11) );
  dest_w = ceil(scale[0]*cro->get_width());
  dest_h = ceil(scale[1]*cro->get_height());
  cro->composite(scr, dest_x, dest_y, dest_w, dest_h, dest_x, dest_y, scale[0], scale[1], Gdk::INTERP_BILINEAR, 255);

  // if cross over ghost, blow up ghost, paint ghosts.
  for ( Entity * it = aliens; it - aliens < aliens_size; it++) {  
    if ( in_bounds(it, cross ) ) { 
      it->state = 0;
      bams.push_back(*it);
      it->speed = 1 + rand() % (level+ 1);
      it->loc.x = area.bounds.left + rand() % (area.width - 60);
      it->loc.y = area.bounds.top;
      score++;
    }
    else if ( (it->loc.y += 5 * it->speed) > area.bounds.bottom - gho[0]->get_height() ) { 
      it->state = 0;
      it->speed = 1 + rand() % (level+ 1);
      it->loc.x = area.bounds.left + rand() % (area.width - 60);
      it->loc.y = area.bounds.top; 
    }
    dest_x = floor( scale[0]*it->loc.x );
    dest_y = floor( scale[1]*it->loc.y );
    dest_w = ceil(scale[0]*gho[0]->get_width());
    dest_h = ceil(scale[1]*gho[0]->get_height());
    gho[(++(it->state)) %= 8]->composite(scr, dest_x, dest_y, dest_w, dest_h, dest_x, dest_y, scale[0], scale[1], Gdk::INTERP_BILINEAR, 255); 
  }

  // paint explosions
  for ( vector<Entity>::iterator it = bams.begin(); it < bams.end(); it++ ) { 
    dest_x = floor( scale[0]*(it->loc.x+10) );
    dest_y = floor( scale[1]*(it->loc.y+20) );
    dest_w = ceil(scale[0]*bam[0]->get_width());
    dest_h = ceil(scale[1]*bam[0]->get_height());
    bam[it->state]->composite(scr, dest_x, dest_y, dest_w, dest_h, dest_x, dest_y, scale[0], scale[1], Gdk::INTERP_BILINEAR, 255);
    if( (it->state++) == 7 ) { it = bams.erase(it); }   
  }

  // paint the local buffer to the pw
  //window->draw_pixbuf(pw.get_style()->get_black_gc(), scr, 0, 0, 0, 0, win_width, win_height, Gdk::RGB_DITHER_NONE, 0, 0);
  scr->render_to_drawable(window, pw.get_style()->get_black_gc(), 0, 0, 0, 0, win_width, win_height, Gdk::RGB_DITHER_NONE, 0, 0);
  //scr->render_to_drawable_alpha(window, 0, 0, 0, 0, win_width, win_height, Gdk::PIXBUF_ALPHA_BILEVEL, 255, Gdk::RGB_DITHER_NONE, 0, 0);
}


void Invaders::resize() {
  // resize the local buffer
  scr = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB,true,8,win_width,win_height);
  scr->fill(0x000000FF);
}

void Invaders::key(GdkEventKey * event){
  switch(event->keyval) {
    case GDK_Pause : paused = paused^0x01; break;
    case GDK_r     : score = 0; break;
    case GDK_R     : score = 0; break;
    default        : break;
  }
}

void Invaders::expose(GdkEventExpose* event) {
  win_width = event->area.width;
  win_height = event->area.height;
  resize(); 
}

extern "C" {
  ModInterface *get_module() { return new Invaders(); };
};

