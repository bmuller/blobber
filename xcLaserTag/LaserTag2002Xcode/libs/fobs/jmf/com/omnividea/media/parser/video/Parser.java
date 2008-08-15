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
import com.moesol.bindings.NativeLibraryFinder;
import javax.media.*;
import javax.media.protocol.*;
import javax.media.format.*;
import java.awt.*;
import com.sun.media.format.*;
import java.nio.ByteBuffer;

public class Parser implements Demultiplexer {

  private native void avInit(String filename);

  private native boolean avOpen(long peer);
  private native boolean avClose(long peer, boolean freePeer);

  private native boolean avIsVideoPresent(long peer);
  private native int avGetWidth(long peer);
  private native int avGetHeight(long peer);
  private native float avGetFrameRate(long peer);
  private native double avGetBitRate(long peer);
  private native double avGetDurationSec(long peer);
  private native boolean avIsSeekable(long peer);
  private native double avGetNextFrameTime(long peer);
  private native boolean avIsAudioPresent(long peer);
  private native double avGetAudioBitRate(long peer);
  private native double avGetAudioSampleRate(long peer);
  private native int avGetAudioChannelNumber(long peer);

  private native double avSetPosition(long peer, double position);
  private native boolean avProcess(
		  long peer, Object outData, long outDataBytes, long length, boolean useNativeBuffers, int frameFormat);
  private native boolean avProcessAudio(
		  long peer, Object outData, long outDataBytes, long length);
  private native int avGetAudioSampleSize(long peer);
  private native double avGetAudioSampleTimestamp(long peer);
  private native double avGetFirstVideoTime(long peer);
  private native double avGetFirstAudioTime(long peer);

  public static native boolean avIsBigEndian();

  private long peer = 0;
  private Object peerSync = new Object();



  protected com.omnividea.media.protocol.DataSource dataSource;
  private ContentDescriptor[] inputContent =
      new ContentDescriptor[] {
      new ContentDescriptor("video.ffmpeg")};

  private javax.media.Track[] tracks = null;
  private Time duration = new Time(0.0);
  private int videoWidth = -1;
  private int videoHeight = -1;
  private double videoBitRate = 0.0;
  private float videoFrameRate = 0;
  private boolean isAudioPresent = false;
  private boolean isVideoPresent = false;
  private int trackNumber = 0;
  private double audioBitRate = 0.0;
  private double audioSampleRate = 0.0;
  private int audioChannelNumber = 0;

  static public double lastAudioTime = 0.0;
  static public double lastVideoTime = 0.0;

  private boolean positionable = true;
  private boolean randomAccess = true;

  static {
  try {
    //System.loadLibrary("fobs4jmf");
    NativeLibraryFinder.loadLibrary(Parser.class, "fobs4jmf");
    System.out.println("Fobs4JMF - Native shared library found");
  }
  catch (UnsatisfiedLinkError e) {
    System.out.println("Fobs4JMF - Native shared library NOT found");
    e.printStackTrace();
    throw new ExceptionInInitializerError(e.getMessage());
  }
}

  public static boolean isBigEndian()
  {
    return avIsBigEndian();
  }


  public Parser() {
    super();
  }
  public synchronized boolean getNextFrame(Object outData, long outDataBytes, long length)
  {
    return avProcess(peer, outData, outDataBytes, length, FobsConfiguration.useNativeBuffers, FobsConfiguration.videoFrameFormat);
  }
  public double getTimestamp()
  {
    return avGetNextFrameTime(peer);
  }
  public int getAudioSampleNumber()
  {
    return avGetAudioSampleSize(peer);
  }
  public synchronized boolean getNextAudioFrame(Object outData, long outDataBytes, long length)
  {
    return avProcessAudio(peer, outData, outDataBytes, length);
  }
  public double getAudioSampleTimestamp()
  {
    return avGetAudioSampleTimestamp(peer);
  }

  public void setSource(DataSource source) throws IncompatibleSourceException {
    // is it our DataSource?
    //if (! (source instanceof com.omnividea.media.protocol.file.DataSource))
	//System.out.println("Coming a "  + source.getContentType() + " file.");
	if(!source.getContentType().equals("video.ffmpeg"))
    {
      IncompatibleSourceException exp = new IncompatibleSourceException("Invalid DataSource");
      exp.printStackTrace();
      throw exp;
    }
    else {
      dataSource = (com.omnividea.media.protocol.DataSource) source;
      //System.out.println("\tPARSER URL: "+dataSource.getUrlName());
      if (dataSource.getUrlName() == null) {
        throw new IncompatibleSourceException("Invalid Datasource");
      }
    } // else is our DS

    //Call ffmpeg for data

    avInit(dataSource.getUrlName());
    synchronized(peerSync) {

      if(avOpen(peer) == false)
		throw new IncompatibleSourceException("Fobs cannot read such url");


	
    duration = new Time(avGetDurationSec(peer));
    trackNumber = 0;
    isVideoPresent = avIsVideoPresent(peer);
    if(isVideoPresent)
    {
        trackNumber++;
        videoWidth = avGetWidth(peer);
        videoHeight = avGetHeight(peer);
        videoBitRate = avGetBitRate(peer);
        videoFrameRate = avGetFrameRate(peer);
    }
    isAudioPresent = avIsAudioPresent(peer);
    if(isAudioPresent)
    {
        trackNumber++;
      audioBitRate = avGetAudioBitRate(peer);
      audioSampleRate = avGetAudioSampleRate(peer);
      audioChannelNumber = avGetAudioChannelNumber(peer);
    }
	

    
	positionable = true;

    randomAccess = avIsSeekable(peer);
    //isAudioPresent = false;
    
    tracks = new javax.media.Track[trackNumber];
    int trackIndex = 0;
    if(isVideoPresent)
    {
        Time firstVideoTime = new Time(avGetFirstVideoTime(peer));
        tracks[trackIndex++] = new VideoTrack(videoWidth, videoHeight, videoFrameRate, duration, firstVideoTime, this);
    }
    if(isAudioPresent)
    {
      Time firstAudioTime = new Time(avGetFirstAudioTime(peer));
      tracks[trackIndex++] = new AudioTrack(audioSampleRate, audioChannelNumber,
                                 videoFrameRate, duration, firstAudioTime, this);
    }
    
    }//synchronized

  }

  public Time getDuration() {
    return duration;
  }
  public ContentDescriptor[] getSupportedInputContentDescriptors() {
    return inputContent;
  }

  public void start()
  {
  }

  public void stop()
  {
  }

  public javax.media.Track[] getTracks() throws java.io.IOException,
      BadHeaderException {
    return tracks;
  }

  public boolean isPositionable() {
    return positionable;
  }

  public boolean isRandomAccess() {
    return randomAccess;
  }

  public Time setPosition(Time where, int rounding) {
    System.out.println("Setting position "+Thread.currentThread());
    double newTime = 0.0;
    synchronized(peerSync) {newTime = avSetPosition(peer, where.getSeconds());}
    return new Time(newTime);
  }

  public Time getMediaTime() {
    //System.out.println("PARSER getMediaTime");
    return new Time(avGetNextFrameTime(peer));
  }

  public String getName() {
    return "FOBS PARSER";
  }

  public void open() {
    System.out.println("Opening "+Thread.currentThread());
    synchronized(peerSync) {avOpen(peer);}
  }
  
  public void reset() {
    System.out.println("Resetting "+Thread.currentThread());
      synchronized(peerSync) {avClose(peer, false);}
      open();
    }

    public void close() {
    System.out.println("Closing "+Thread.currentThread());
      synchronized(peerSync) {avClose(peer, true);}
    }

  public Object[] getControls() {
    return null;
  }

  public Object getControl(String s) {
    return null;
  }

}
