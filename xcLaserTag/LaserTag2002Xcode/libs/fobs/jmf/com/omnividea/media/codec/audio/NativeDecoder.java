/******************************************************************************
* FOBS Java CrossPlatform JMF PlugIn
* Copyright (c) 2004 Omnividea Multimedia S.L
* Coded by Jose San Pedro Wandelmer
*
*    This file is part of FOBS.
*
*    FOBS is free software; you can redistribute it and/or modify
*    it under the terms of the GNU Lesser General Public License as
*    published by the Free Software Foundation; either version 2.1 
*    of the License, or (at your option) any later version.
*
*    FOBS is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Lesser General Public License for more details.
*
*    You should have received a copy of the GNU Lesser General Public
*    License along with FOBS; if not, write to the Free Software
*    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

package com.omnividea.media.codec.audio;

import javax.media.*;
import javax.media.protocol.*;
import javax.media.format.*;
import java.awt.*;
import com.sun.media.format.*;

public class NativeDecoder   extends com.sun.media.codec.audio.AudioCodec {
  public NativeDecoder() {
    inputFormats = new Format[] { new AudioFormat("FFMPEG_AUDIO") };
  }

  public String getName() {
    return "NULL Fobs Audio Codec";
  }


  public Format [] getSupportedOutputFormats(Format in) {

       if (in == null)
           return new Format[] { new AudioFormat(AudioFormat.LINEAR) };

       AudioFormat af =(AudioFormat) in;

       return new Format[] {
           new WavAudioFormat(AudioFormat.LINEAR,
                              af.getSampleRate(),
                              af.getSampleSizeInBits(),
                              af.getChannels(),
                              af.getFrameSizeInBits(),
                              (int)(af.getFrameSizeInBits() * af.getSampleRate()/8),
                              af.getEndian(),
                              af.getSigned(),
                              (float)af.getFrameRate(), // No FRAME_RATE specified
                              af.getDataType(),
                              new byte[0])
       };
     }

     public Format setInputFormat(Format format) {

      if (super.setInputFormat(format) != null) {

        AudioFormat af =(AudioFormat)format;

        outputFormat =            new WavAudioFormat(AudioFormat.LINEAR,
                              af.getSampleRate(),
                              af.getSampleSizeInBits(),
                              af.getChannels(),
                              af.getFrameSizeInBits(),
                              (int)(af.getFrameSizeInBits() * af.getSampleRate()/8),
                              af.getEndian(),
                              af.getSigned(),
                              (float)af.getFrameRate(), // No FRAME_RATE specified
                              af.getDataType(),
                              new byte[0]);

        return format;
      }
      else
      {
        return null;
      }

    }

     public void open() {

     }
     
     public void close() {

     }

     public int process(Buffer inputBuffer, Buffer outputBuffer) {
         if (!checkInputBuffer(inputBuffer) ) {
             return BUFFER_PROCESSED_FAILED;
         }

         if (isEOM(inputBuffer) ) {
             propagateEOM(outputBuffer);
             return BUFFER_PROCESSED_OK;
         }

         Object outData = outputBuffer.getData();
         outputBuffer.setData(inputBuffer.getData());
         inputBuffer.setData(outData);
         outputBuffer.setLength(inputBuffer.getLength());
         outputBuffer.setFormat(outputFormat);
         outputBuffer.setOffset(inputBuffer.getOffset());
         return BUFFER_PROCESSED_OK;


     }

 }
