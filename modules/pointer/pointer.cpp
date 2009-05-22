#include "pointer.h"

Pointer::Pointer() : ModInterface("Pointer"), mod(POINTER_MODE_TIMED), loc(0, 0) {
  clk = clock();
} 
Pointer::~Pointer() {}

void Pointer::init(Camarea &area, ProjectionWindow &pw) {
  // register some callbacks for Gtk events on the ProjectionWindow
  pw.signal_key_press_event().connect_notify(sigc::mem_fun(*this, &Pointer::key));
  pw.signal_expose_event().connect_notify(sigc::mem_fun(*this, &Pointer::expose));
  
  // register poi
  register_poi(area, 1);
}


void Pointer::update(Camarea &area, ProjectionWindow &pw) {
  switch( mod ) {
    case POINTER_MODE_TIMED : update_mode_timed( area, pw ); break;
  }
}

void Pointer::update_mode_timed( Camarea &area, ProjectionWindow &pw ) {
  COORD t;
  vector<PIXEL> poi;
  get_poi(area, poi);
  if ( poi.size() != 0 ) {
    pw.translate_coordinates(poi[0].coord,t);
    Display * dpy = XOpenDisplay( NULL );
    XTestFakeMotionEvent( dpy, DefaultScreen( dpy ), t.x, t.y, CurrentTime );

    // if new location is  > 5 away, start clock over, reset loc
    if( loc.distance_from( poi[0].coord ) > 5 ) {
      clk = clock();
      loc.copy( poi[0].coord );
    }

    // if 1.0 seconds have passed and pointer has not moved, send click
    if( ( clock() - clk ) / CLOCKS_PER_SEC > 1.0 ) {
      XTestFakeButtonEvent( dpy, Button1, True, CurrentTime );
      XTestFakeButtonEvent( dpy, Button1, False, CurrentTime );
      clk = clock();
    }

    XCloseDisplay( dpy );
  }
}

void Pointer::key(GdkEventKey * event){
//  switch(event->keyval) {
//    default        : break;
//  }
}

void Pointer::expose(GdkEventExpose* event) {}

extern "C" {
  ModInterface *get_module() { return new Pointer(); };
};

