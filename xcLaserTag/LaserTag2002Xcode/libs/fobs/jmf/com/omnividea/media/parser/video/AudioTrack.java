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

package com.omnividea.media.parser.video;

import com.omnividea.FobsConfiguration;

import javax.media.*;
import javax.media.protocol.*;
import javax.media.format.*;
import java.awt.*;
import com.sun.media.format.*;
import java.nio.ByteBuffer;

class AudioTrack implements javax.media.Track
{
  private AudioFormat outFormat;
  private boolean enabled = false;
  private Time startTime = new Time(0.0);
  private Time duration =  new Time(0.0);
  private Parser parser = null;
  private double frameRate = 0.0;
  private long lts = 0, lt = 0;
  public AudioTrack(double sampleRate, int channelNumber, double frameRate, Time duration, Time startTime, Parser parser)
  {
    int frameSizeInBits = channelNumber * 16;
    int avgBytesPerSec = (int)(channelNumber * 2 * sampleRate);

/*    outFormat = new WavAudioFormat(AudioFormat.LINEAR,
                                sampleRate,
                                16,
                                channelNumber,
                                frameSizeInBits,
                                avgBytesPerSec,
                                AudioFormat.BIG_ENDIAN,
                                AudioFormat.SIGNED,
                                Format.NOT_SPECIFIED, // No FRAME_RATE specified
                                Format.byteArray,
                                new byte[0]);*/

    outFormat = new WavAudioFormat("FFMPEG_AUDIO",
                             sampleRate,
                             16,
                             channelNumber,
                             frameSizeInBits,
                             avgBytesPerSec,
                             parser.isBigEndian()?AudioFormat.BIG_ENDIAN:AudioFormat.LITTLE_ENDIAN,
                             AudioFormat.SIGNED,
                             Format.NOT_SPECIFIED, // No FRAME_RATE specified
                             Format.byteArray,
                             new byte[0]);

    this.duration = duration;
    enabled = true;
    this.parser = parser;
    this.frameRate = frameRate;
    this.startTime = startTime;
  }
  public Format getFormat()
  {
    return outFormat;
  }
  public void setEnabled(boolean t){enabled = t;}
  public boolean isEnabled(){return enabled;}

  public Time getStartTime(){return startTime;}

  public void readFrame(Buffer buffer)
  {
    if (buffer == null)
        return;

    if (!isEnabled()) {
        buffer.setDiscard(true);
        return;
    }

    buffer.setFormat(outFormat);
    Object obj = buffer.getData();
    byte[] data;
    long location;
    int needDataSize;

    needDataSize = outFormat.getChannels() * outFormat.getSampleSizeInBits() * 4000;


    if  ( (obj == null) ||
          (! (obj instanceof int[]) ) ||
          ( ((byte[])obj).length < needDataSize) ) {
        data = new byte[needDataSize];
        buffer.setData(data);
    } else {
        data = (byte[]) obj;
    }

    if(parser.getNextAudioFrame(data, 0, needDataSize))
    {
      int size = parser.getAudioSampleNumber();
      buffer.setOffset(0);
      buffer.setLength(size);
      //buffer.setDuration((long)(size / outFormat.getSampleSizeInBits() * 8 / outFormat.getSampleRate() / outFormat.getChannels() * 1000000000));
      double audioTime = parser.getAudioSampleTimestamp();

      long ts = (long)(audioTime*1000000000);
      buffer.setTimeStamp(ts);
      
      long tmp = System.currentTimeMillis();
      //System.out.println("ATS: "+ts+" Delta:"+(ts-lts)/1000000+" T:"+(tmp-lt));
      lts = ts;
      lt = tmp;
    }
    else
    {
        buffer.setLength(0);
        buffer.setEOM(true);
    }
  }

  public int mapTimeToFrame(Time t){
    return 0;}
  public Time mapFrameToTime(int frameNumber){
    return null;
  }

  public void setTrackListener(TrackListener listener){}
  public Time getDuration(){return parser.getDuration();}

}