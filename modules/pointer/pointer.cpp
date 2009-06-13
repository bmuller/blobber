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
  // this branch is for future extensions, i.e. how clicks should be
  // made. POINTER_MODE_TIMED means that a click signal is sent when 
  // the pointer has not been moved for one second
  switch( mod ) {
    case POINTER_MODE_TIMED : update_mode_timed( area, pw ); break;
  }
}

void Pointer::update_mode_timed( Camarea &area, ProjectionWindow &pw ) {
  COORD t;
  BVector<PIXEL> poi;
  get_poi(area, poi);
  if ( poi.size() != 0 ) {
    translate_coordinates( area, poi[0].coord, t ); 
    Display * dpy = XOpenDisplay( NULL );
    XTestFakeMotionEvent( dpy, DefaultScreen( dpy ), t.x, t.y, CurrentTime );

    //Haven't got the clicking part down yet
    string delString, clkString;
    num_to_string( loc.distance_from( poi[0].coord ), delString );
    num_to_string( ( (float) ( clock() - clk ) ) / CLOCKS_PER_SEC, clkString );
    debug( "pointer: delta = " + delString + " clock = " + clkString );

    // if new location is  > 20 away, start clock over, reset loc
    if( loc.distance_from( poi[0].coord ) > 20.0 ) {
      clk = clock();
      loc.copy( poi[0].coord );
    }

    // if 1.0 seconds have passed and pointer has not moved (much), send click
    if( ( (float) ( clock() - clk ) ) / CLOCKS_PER_SEC > 1.0 ) {
      XTestFakeButtonEvent( dpy, Button1, True, CurrentTime );
      XTestFakeButtonEvent( dpy, Button1, False, CurrentTime );
      clk = clock();
    }

    XCloseDisplay( dpy );
  }
}

/// Converts camera coordinates to screen coordinates
void Pointer::translate_coordinates( Camarea &area, COORD &camera, COORD &screen ) {
  screen.x = (int) ( ( ( (float) camera.x - area.bounds.left ) / ( (float) area.bounds.width() ) ) * scw );
  screen.y = (int) ( ( ( (float) camera.y - area.bounds.top ) / ( (float) area.bounds.height() ) ) * sch );
}

void Pointer::key(GdkEventKey * event){}

extern "C" {
  ModInterface *get_module() { return new Pointer(); };
};

