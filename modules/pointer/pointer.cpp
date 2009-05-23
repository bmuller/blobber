#include "pointer.h"

Pointer::Pointer() : ModInterface("Pointer"), mod(POINTER_MODE_TIMED), loc(0, 0) {
  Display * dpy = XOpenDisplay( NULL );
  sch = DisplayHeight( dpy, DefaultScreen( dpy ) );
  scw = DisplayWidth( dpy, DefaultScreen( dpy ) );
  XCloseDisplay( dpy );
  clk = clock();
} 

Pointer::~Pointer() {}

void Pointer::init(Camarea &area, ProjectionWindow &pw) {
  // register some callbacks for Gtk events on the ProjectionWindow
  pw.signal_key_press_event().connect_notify(sigc::mem_fun(*this, &Pointer::key));
  
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
    translate_coordinates( area, poi[0].coord, t ); 
    Display * dpy = XOpenDisplay( NULL );
    XTestFakeMotionEvent( dpy, DefaultScreen( dpy ), t.x, t.y, CurrentTime );

    //Haven't got the clicking part down yet

    // if new location is  > 15 away, start clock over, reset loc
    //if( loc.distance_from( poi[0].coord ) > 15.0 ) {
    //  clk = clock();
    //  loc.copy( poi[0].coord );
    //}

    // if 1.0 seconds have passed and pointer has not moved, send click
    //if( ( (float) ( clock() - clk ) ) / CLOCKS_PER_SEC > 1.0 ) {
    //  XTestFakeButtonEvent( dpy, Button1, True, CurrentTime );
    //  XTestFakeButtonEvent( dpy, Button1, False, CurrentTime );
    //  clk = clock();
    //}

    XCloseDisplay( dpy );
  }
}

void Pointer::translate_coordinates( Camarea &area, COORD &camera, COORD &screen ) {
  screen.x = (int) ( ( ( (float) camera.x ) / ( (float) area.dimensions.width ) ) * scw );
  screen.y = (int) ( ( ( (float) camera.y ) / ( (float) area.dimensions.height ) ) * sch );
}

void Pointer::key(GdkEventKey * event){}

extern "C" {
  ModInterface *get_module() { return new Pointer(); };
};

