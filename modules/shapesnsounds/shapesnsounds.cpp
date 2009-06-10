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

ShapesNSounds::ShapesNSounds() : ModInterface("ShapesNSounds", "Participatory music making module"), missing_counter(0) {
  // can't do much here cause we can't open a device until we're inited
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
    throw ModuleRuntimeException("Could not open audio stream for Theremin module");
  } catch ( StkError & ) {
    throw ModuleRuntimeException("Could not open audio stream for Theremin module");
  }
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


void Theremin::note_off() {  
  if(!note_is_on) return;
  mod_debug("Note off");
  data.instrument->noteOff(0.5);
  note_is_on = false;
  dac.stopStream();
};


void Theremin::projection_window_exposed(ProjectionWindow &pw) {
  pw.get_drawing_area_dimensions(dimensions);
};


void Theremin::update(Camarea &area, ProjectionWindow &pw) {
  BVector<PIXEL> poi;
  get_poi(area, poi);

  if(poi.size() == 0) {
    missing_counter++;
    if(missing_counter > 5)
      note_off();
  } else {
    missing_counter = 0;
    // make noise if laser has moved - frequency is based on x coordinate
    if(poi[0].coord.distance_from(lastpoint) > 5) {
      double frequency = min_frequency + ((max_frequency - min_frequency) * ((double) poi[0].coord.x / (double) dimensions.width));
      note_on(frequency);
    }
    lastpoint.copy(poi[0].coord);
  }
};


extern "C" {
  ModInterface *get_module() {
    return new Theremin();
  };
};
