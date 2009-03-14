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

#include "theremin.h"

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
    /*
    if ( ++data->counter % 2000 == 0 ) {
      data->scaler += 0.025;
      data->instrument->setFrequency( data->frequency * data->scaler );
    }
    */
  }

  //if ( data->counter > 80000 )
  //  data->done = true;

  return 0;
}

/////////////////////////////////////////////////////////////////////

Theremin::Theremin() : ModInterface("Theremin", "Theremin music making module"), note_is_on(false) {
  // Set the global sample rate and rawwave path before creating class instances.
  Stk::setSampleRate( 44100.0 );
  Stk::setRawwavePath( "/home/bmuller/blobber/modules/theremin" );

  // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
  parameters.deviceId = dac.getDefaultOutputDevice();
  parameters.nChannels = 1;
  RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  bufferFrames = RT_BUFFER_SIZE;
  try {
    dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&data );
    data.instrument = new BeeThree();
    dac.startStream();
  } catch ( RtError& error ) {
    error.printMessage();
    throw ModuleRuntimeException("Could not open audio stream for Theremin module");
  } catch ( StkError & ) {
    throw ModuleRuntimeException("Could not open audio stream for Theremin module");
  }
  debug("in theremin constructor");
};

void Theremin::init(Camarea &area, ProjectionWindow &pw) {
  /*
  debug("initing theremin module");
  // use default CRANGE criteria
  register_poi(area, 2);
  note_on(440.0);
  Stk::sleep(3000);
  note_off();
  */
};

Theremin::~Theremin() {
/*
  debug("HERE OK");
  // Shut down the callback and output stream.
  try {
    dac.closeStream();
  }
  catch ( RtError &error ) {
    error.printMessage();
    throw ModuleRuntimeException("Problem shutting down the Theremin module");
  }

  delete data.instrument;
  */
};

void Theremin::note_on(double frequency) {
  if(note_is_on)
    note_off();
  data.frequency = frequency;
  data.instrument->noteOn( data.frequency, 0.5 );
  note_is_on = true;
};

void Theremin::note_off() {
  if(!note_is_on) return;
  data.instrument->noteOff(0.5);
  note_is_on = false;
};

void Theremin::projection_window_exposed(ProjectionWindow &pw) {

};

void Theremin::update(Camarea &area, ProjectionWindow &pw) {

};

extern "C" {
  ModInterface *get_module() {
    return new Theremin();
  };
};
