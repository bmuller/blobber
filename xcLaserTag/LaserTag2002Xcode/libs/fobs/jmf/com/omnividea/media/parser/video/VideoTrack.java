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

/*
 * Created by Jose San Pedro Wandelmer
 * ChangeLog:
 *  Robert Hastings 2007/01/04
 *   - Native Library Location routines
 *   - Improvements to frame buffer management
 */
 
package com.omnividea.media.parser.video;

import com.omnividea.FobsConfiguration;

import javax.media.*;
import javax.media.protocol.*;
import javax.media.format.*;
import java.awt.*;
import com.sun.media.format.*;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.ByteOrder;

class VideoTrack implements javax.media.Track
{
  static private int bMask = 0x0000ff00;
  static private int gMask = 0x00ff0000;
  static private int rMask = 0xff000000;

  private VideoFormat outFormat;
  private boolean enabled = false;
  private Time startTime = new Time(0.0);
  private Time duration =  new Time(0.0);
  private Parser parser = null;
  private ByteBuffer byteBuffer = null;
  private IntBuffer intBuffer = null;
  private long timestamp = 0;
  
  private long lts = 0, lt = 0;
  private long accTime = 0;
  private int[] pixels; // Instead of newing these each time we will just keep using these

  public VideoTrack(int videoWidth, int videoHeight, float frameRate, 
                    Time duration, Time startTime, Parser parser)
  {
      /*outFormat = new RGBFormat (
      new Dimension(videoWidth,videoHeight),
      videoWidth * videoHeight, int[].class,
      frameRate,
      32,
      rMask, gMask, bMask,
      1,videoWidth,
      Format.FALSE, // flipped
      Format.NOT_SPECIFIED
      );*/
    outFormat = new AviVideoFormat(
        "ffmpeg_video",
        new Dimension(videoWidth, videoHeight),
        videoWidth * videoHeight, byte[].class,
        (float)frameRate, 0, 0, 0, 0, 0, 0, 0, new byte[0]);
 
    this.duration = duration;
    enabled = true;
    this.parser = parser;
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
    int[] data;
    long location;
    int needDataSize;

    /*
    if (location < minLocation) {
        buffer.setDiscard(true);
        return;
    } else if (location >= maxLocation) {
        buffer.setLength(0);
        buffer.setEOM(true);
        return;
    }
    */

    needDataSize = outFormat.getSize().width*outFormat.getSize().height;


    /*if  ( (obj == null) ||
          (! (obj instanceof int[]) ) ||
          ( ((int[])obj).length < needDataSize) ) {
        int []pixels = (int [])FobsConfiguration.properties.get("BufferedImageIntBuffer");
        if(pixels == null)
        {
            data = new int[needDataSize];
        }
        else
        {
            data  = pixels;
        }
        buffer.setData(data);
    } else {
        data = (int[]) obj;
    }*/
    
    if  ( dataNotOkay(obj, needDataSize) ) {
    	if (pixels == null) {
    		pixels = new int[needDataSize];
    	}
    	data = pixels;
        buffer.setData(data);
    } else {
        data = (int[]) obj;
    }
    
    boolean flag;
    if(FobsConfiguration.useNativeBuffers)
    {
        if(byteBuffer == null)
        {
            byteBuffer = ByteBuffer.allocateDirect(4*needDataSize);
            if(Parser.avIsBigEndian()==false) byteBuffer.order(ByteOrder.LITTLE_ENDIAN);            
        }
        //assert(byteBuffer!=null);
        flag = parser.getNextFrame(byteBuffer, 0, needDataSize);
        if(flag)
        {
            byteBuffer.asIntBuffer().get(data);
        }
    }
    else
    {
        flag = parser.getNextFrame(data, 0, needDataSize);
    }

    

    long tmp = System.currentTimeMillis();
    if(flag)
    {
        accTime += System.currentTimeMillis() - tmp;
        //System.out.println("AccTime: "+accTime);
      buffer.setOffset(0);
      buffer.setLength(needDataSize);
      //buffer.setDuration((long)(1000000000.0/outFormat.getFrameRate()));
      double videoTime = parser.getTimestamp();
      long ts = (long)((videoTime)*1000000000);
      buffer.setTimeStamp(ts);
      //System.out.println("VTS: "+ts+" Delta:"+(ts-lts)/1000000+" T:"+(tmp-lt));
      lts = ts;
      lt = tmp;
    }
    else
    {
        buffer.setLength(0);
        buffer.setEOM(true);
    }
  }

  private boolean dataNotOkay(Object obj, int needDataSize) {
	return (obj == null) ||
	      (! (obj instanceof int[]) ) ||
	      ( ((int[])obj).length < needDataSize);
  }

  public int mapTimeToFrame(Time t){
	  System.out.println("FobsVideoTrack: mapTimeToFrame");
    return 0;
  }
  public Time mapFrameToTime(int frameNumber){
	  System.out.println("FobsVideoTrack: mapFrameToTime");
    return null;
  }

  public void setTrackListener(TrackListener listener){}
  public Time getDuration(){return parser.getDuration();}

}
