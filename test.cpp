#include "nerdtag.h"
#include <SDL/SDL.h>

class CamScreen {
public:
  SDL_Surface *screen;
  SDL_Surface *image;
  FrameGrabber *fg;
  Frame * frame;
  int bpp;

  CamScreen(string device) {
    fg = new FrameGrabber(device);
    frame = fg->makeFrame();
    switch ( frame->format ) {
    case VIDEO_PALETTE_RGB24:
	bpp = 24;
	break;
    case VIDEO_PALETTE_RGB32:
      bpp = 32;
      break;
    default:
      cerr << "Unsupported frame format: " << frame->format << endl;
    }
  };
  ~CamScreen() { delete frame; delete fg; };
  void update() { fg->grabFrame(frame); };
};


int main( int argc, char* argv[] ) {
  CamScreen cs("/dev/video0");

  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    cerr << "Failed to initialise SDL: " << SDL_GetError() << endl;
    return -1;
  }
  SDL_WM_SetCaption( "camview", NULL );
  cs.screen = SDL_SetVideoMode(cs.fg->caps.maxwidth, cs.fg->caps.maxheight, cs.bpp, SDL_SWSURFACE );
  if ( cs.screen == NULL ) {
    cerr << "Failed to set video mode: %s" << SDL_GetError() << endl;
    return -1;
  }

  unsigned int bmask = 0x000000ff;
  unsigned int gmask = 0x0000ff00;
  unsigned int rmask = 0x00ff0000;
  unsigned int amask = 0xff000000;
  cs.image = SDL_CreateRGBSurfaceFrom(cs.frame->data, cs.frame->width, cs.frame->height, cs.bpp, cs.frame->width * 4, \
				      rmask, gmask, bmask, amask);
  if ( cs.image == NULL ) {
    cerr << "Failed to create RGB surface! " << SDL_GetError() << endl;
    return 1;
  }

  // Ignore alpha channel, go opaque                                                                                      
  SDL_SetAlpha( cs.image, 0, 0 );

  SDL_Event event;
  int running = 1;
  while( running ) {
    while ( SDL_PollEvent(&event) ) {
      if ( event.type == SDL_QUIT ) 
	running = 0;     
      if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE )
	running = 0;

      cs.update();
      if (SDL_BlitSurface( cs.image, NULL, cs.screen, NULL ) < 0 ) {
	cerr << "Cannot blit!" << SDL_GetError() << endl;
	return 1;
      }
      SDL_UpdateRect( cs.screen, 0, 0, 0, 0 );
    }
  }

  return 0;
}
