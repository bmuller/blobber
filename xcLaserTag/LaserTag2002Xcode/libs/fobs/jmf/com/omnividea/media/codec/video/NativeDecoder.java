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

package com.omnividea.media.codec.video;

import javax.media.*;
import javax.media.protocol.*;
import javax.media.format.*;
import com.ibm.media.codec.video.*;
import java.awt.*;
import com.sun.media.format.*;

public class NativeDecoder   extends VideoCodec {
  static private int bMask = 0x0000ff;
  static private int gMask = 0x00ff00;
  static private int rMask = 0xff0000;


  public NativeDecoder() {
    supportedInputFormats = new VideoFormat[] { new VideoFormat("ffmpeg_video") };
    defaultOutputFormats = new VideoFormat[] {
        new RGBFormat()};
    PLUGIN_NAME = "NULL Fobs Video Codec";
  }

  public Format [] getMatchingOutputFormats(Format in) {
       if (in == null)
           return new VideoFormat[] { new RGBFormat() };

       VideoFormat vf =(VideoFormat) in;
       RGBFormat f = new RGBFormat (
             new Dimension(vf.getSize()),
             vf.getSize().width*vf.getSize().height,
             int[].class,
             vf.getFrameRate(),
             32,
             rMask, gMask, bMask,
             1,vf.getSize().width,
             Format.FALSE, // flipped
             RGBFormat.BIG_ENDIAN
             );

       supportedOutputFormats = new VideoFormat[] {f};
       return supportedOutputFormats;
     }

      public Format setInputFormat(Format format)
      {
         if (super.setInputFormat(format) != null)
         {
           VideoFormat ivf = (VideoFormat)format;
           int videoWidth = ivf.getSize().width;
           int videoHeight = ivf.getSize().height;

           outputFormat = new RGBFormat (
             ivf.getSize(),
             ivf.getSize().width*ivf.getSize().height,
             int[].class,
             ivf.getFrameRate(),
             32,
             rMask, gMask, bMask,
             1,ivf.getSize().width,
             Format.FALSE, // flipped
             RGBFormat.BIG_ENDIAN //NOT_SPECIFIED
          );

          return format;
        }
        else{
         return null;
       }
     }

     public void open() {}
     public void close() {}

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
