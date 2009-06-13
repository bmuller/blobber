/*
    Blobber is webcam based blob tracking software
    Copyright (C) 2008 Charleston, SC Linux Users Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// based loosely on bethree.cpp STK tutorial program

#include "shapesnsounds.h"

using namespace blobber;


// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData ) {
  TickData *data = (TickData *) userData;
  register StkFloat *samples = (StkFloat *) outputBuffer;

  for ( unsigned int i=0; i<nBufferFrames; i++ ) {
    *samples++ = data->instrument->tick();
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////

ShapesNSounds::ShapesNSounds() : ModInterface("ShapesNSounds", "ShapesNSounds music making module"), note_is_on(false), missing_counter(0), xhairs(PROPORTION(0.02,0.02),COORD(0,0),WHITE) {
  // can't do much here cause we can't open a device until we're inited
  lastpoint.x = 0;
  lastpoint.y = 0;
};


void ShapesNSounds::init(Camarea &area, ProjectionWindow &pw) {
  // use default CRANGE criteria
  register_poi(area, 2);

  // Load config information
  string smin_frequency, smax_frequency;
  config_get_set("min_frequency", smin_frequency, "55.0");
  config_get_set("max_frequency", smax_frequency, "1760.0");
  min_frequency = string_to_double(smin_frequency);
  max_frequency = string_to_double(smax_frequency);

  // Set the global sample rate and rawwave path before creating class instances.
  Stk::setSampleRate( 44100.0 );
  Stk::setRawwavePath( string(DATAROOTDIR) + "/libs/stk" );

  // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 1;
  RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  bufferFrames = RT_BUFFER_SIZE;
  try {
    dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&data );
    data.instrument = new BeeThree();
  } catch ( RtError& error ) {
    error.printMessage();
    throw ModuleRuntimeException("Could not open audio stream for ShapesNSounds module");
  } catch ( StkError & ) {
    throw ModuleRuntimeException("Could not open audio stream for ShapesNSounds module");
  }


  DIMENSION visible_dimensions;
  pw.get_drawing_area_dimensions(visible_dimensions);

  COORD middle(visible_dimensions.width/2, visible_dimensions.height/2);
  shapes.push_back(new MovableSquare(PROPORTION(0.1, 0.1), middle, BLUE));

  middle.x += 20;
  middle.y += 20;
  shapes.push_back(new MovableCircle(PROPORTION(0.15, 0.15), middle, RED));

  middle.x -= 40;
  middle.y -= 40;
  shapes.push_back(new MovableSquare(PROPORTION(0.12, 0.12), middle, GREEN));

  middle.x += 20;
  shapes.push_back(new MovableCircle(PROPORTION(0.1, 0.1), middle, LIGHT_BLUE));

  middle.y += 20;
  shapes.push_back(new MovableCircle(PROPORTION(0.13, 0.13), middle, BROWN));
};


ShapesNSounds::~ShapesNSounds() {
  try {
    dac.closeStream();
  }
  catch ( RtError &error ) {
    error.printMessage();
    throw ModuleRuntimeException("Problem shutting down the ShapesNSounds module");
  }

  delete data.instrument;

  for(unsigned int i=0; i<shapes.size(); i++)
    delete shapes[i];
};


void ShapesNSounds::note_on(double frequency) {
  if(note_is_on)
    note_off();

  string sfreq;
  num_to_string(frequency, sfreq);
  mod_debug("Note on at " + sfreq + "hz");

  dac.startStream();
  data.frequency = frequency;
  data.instrument->noteOn( data.frequency, 0.5 );
  note_is_on = true;
};


void ShapesNSounds::note_off() {  
  if(!note_is_on) return;
  mod_debug("Note off");
  data.instrument->noteOff(0.5);
  note_is_on = false;
  dac.stopStream();
};


void ShapesNSounds::projection_window_exposed(ProjectionWindow &pw) {
  pw.get_drawing_area_dimensions(dimensions);

  for(unsigned int i=0; i<shapes.size(); i++)
    shapes[i]->paint(pw);
  xhairs.paint(pw);
};


void ShapesNSounds::update(Camarea &area, ProjectionWindow &pw) {
  BVector<PIXEL> poi;
  get_poi(area, poi);

  // no points means nothing is selected, return 
  if(poi.size() == 0) {
    // if we just went blank, get rid of old crosshairs  
    // we should repaint object if it was just selected, or paint  
    // old crosshairs black if nothing was selected    
    if(missing_counter == 0) {
      bool found = false;
      for(unsigned int i=0; (i<shapes.size() && !found); i++) {
        if(shapes[i]->selected) {
          shapes[i]->paint(pw);
          found = true;
        }
      }
      if(!found)
        xhairs.clear(pw);
    } else if(missing_counter > 5) {
      note_off();
    }
    missing_counter++;
    return;
  }

  xhairs.move(poi[0].coord, pw);

  // clear selected attr if we've missed the laser for 3 or more iterations; 
  if(missing_counter > 3) {
    for(unsigned int i=0; i<shapes.size(); i++)
      shapes[i]->selected = false;
    xhairs.move(poi[0].coord, pw);
  }

  missing_counter = 0;

  // for each point   
  bool isselected = false;
  for(unsigned int i=0; i<shapes.size(); i++) {
    // if previously selected, move, then update all the other shapes 
    // (in case we were occluding one and need to redraw)                                
    if(shapes[i]->selected) {
      shapes[i]->move(poi[0].coord, pw);

      note_off();
      double frequency = min_frequency + ((max_frequency - min_frequency) * ((double) shapes[i]->center.x / (double) dimensions.width));
      note_on(frequency);

      for(unsigned int j=0; j<shapes.size(); j++)
        shapes[j]->paint(pw);
      // make sure selected ends up in front                                               
      shapes[i]->paint(pw);
      isselected = true;
      break;
    }
  }

  // could be a new selection - but only one can be selected 
  if(!isselected) {
    for(unsigned int i=0; i<shapes.size(); i++) {
      shapes[i]->paint(pw);
      // only one new one can be selected 
      if((shapes[i]->selected = shapes[i]->in_bounds(poi[0].coord)))
        break;
    }
  }

};


extern "C" {
  ModInterface *get_module() {
    return new ShapesNSounds();
  };
};
