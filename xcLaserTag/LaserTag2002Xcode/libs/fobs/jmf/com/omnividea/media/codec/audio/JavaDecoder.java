/******************************************************************************
* FOBS Java JMF Stand Alone Codec - Java Part
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
 */
 
package com.omnividea.media.codec.audio;

import javax.media.*;
import javax.media.format.*;
import javax.media.format.*;
import com.sun.media.*;
import com.sun.media.format.*;
import com.sun.media.controls.*;
import com.ibm.media.codec.audio.*;
import java.awt.Dimension;
import javax.media.rtp.*;
import javax.media.control.*;
import java.awt.*;
import java.awt.event.*;
import com.moesol.bindings.NativeLibraryFinder;


/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: </p>
 * @author not attributable
 * @version 1.0
 */

public class JavaDecoder extends AudioCodec {

  //Native Methods
  private native static boolean isBigEndian();
  private native void    init();
  private native boolean open_codec(long peer, String codec);
  private native boolean close_codec(long peer);
  private native void init_decoding(long peer, int width, int height);
  private native boolean convert( long peer,
                                  Object inData, long inDataBytes, int inOffset,
                                  Object outData,  long outDataBytes, long length, double dts);
  private native int lastAudioSize(long peer);

  private long peer = 0;
  private boolean outputDone = false;


  static final private boolean DEBUG = false;
  static private boolean bigEndian = true;

  ////////////////////////////////////////////////////////////////////////////
  // Variables

  static {
    try {
      //System.loadLibrary("fobs4jmf");
      NativeLibraryFinder.loadLibrary(JavaDecoder.class, "fobs4jmf");
      System.out.println("Fobs4JMF - Native shared library found");
      bigEndian = isBigEndian();

      if(bigEndian == false)
      {
        System.out.println("Little Endian");
      }
      else
      {
          System.out.println("Big Endian");
      }

    }
    catch (UnsatisfiedLinkError e) {
      System.out.println("Fobs4JMF - Native shared library NOT found");
      e.printStackTrace();
      throw new ExceptionInInitializerError(e.getMessage());
    }
  }


  public JavaDecoder() {
    System.out.println("AVCODEC: Constructor");
    supportedInputFormats = new AudioFormat[] {
        new AudioFormat("twos"), 
        new AudioFormat("raw "), 
        new AudioFormat("ulaw"), 
        new AudioFormat("alaw"), 
        new AudioFormat("ima4"), 
        new AudioFormat("MAC3"), 
        new AudioFormat("MAC6"), 
        new AudioFormat(".mp3"), 
        new AudioFormat("mp4a"), 
        new AudioFormat("samr"), 
        new AudioFormat("sawb"), 
        new AudioFormat("alac")
    };
    defaultOutputFormats = new AudioFormat[] {
        new WavAudioFormat("")};
    PLUGIN_NAME = "Fobs Audio Decoder";

  }

  protected Format[] getMatchingOutputFormats(Format in) {
    System.out.println("AVCODEC: getMatchingOutputFormats");

       if (in == null)
           return new Format[] { new AudioFormat(AudioFormat.LINEAR) };

       AudioFormat af =(AudioFormat) in;

       supportedOutputFormats = new AudioFormat[] {
           new WavAudioFormat(AudioFormat.LINEAR,
                              af.getSampleRate(),
                              af.getSampleSizeInBits(),
                              af.getChannels(),
                              af.getFrameSizeInBits(),
                              (int)(af.getFrameSizeInBits() * af.getSampleRate()/8.0),
                              af.getEndian(),
                              af.getSigned(),
                              (float)af.getFrameRate(), // No FRAME_RATE specified
                              af.getDataType(),
                              new byte[0])
       };

    
    return supportedOutputFormats;
  }



  /**
   * Set the data input format.
   * @return false if the format is not supported.
   */
  public Format setInputFormat(Format format) {
    System.out.println("AVCODEC: setInputFormat");
      if (super.setInputFormat(format) != null) {

        AudioFormat af =(AudioFormat)format;

        outputFormat =            new WavAudioFormat(AudioFormat.LINEAR,
                              af.getSampleRate(),
                              af.getSampleSizeInBits(),
                              af.getChannels(),
                              af.getFrameSizeInBits(),
                              (int)(af.getFrameSizeInBits() * af.getSampleRate()/8.0),
                              af.getEndian(),
                              af.getSigned(),
                              (float)af.getFrameRate(), // No FRAME_RATE specified
                              af.getDataType(),
                              new byte[0]);

        return format;
    }
    else
      return null;
  }

    public void open() throws ResourceUnavailableException {
      System.out.println("AVCODEC: open");
      init();
      if (peer == 0)
          throw new ResourceUnavailableException("Unable to initialize");


      if (inputFormat == null)
          throw new ResourceUnavailableException("No input format selected");

      if (outputFormat == null)
          throw new ResourceUnavailableException("No output format selected");

//      init_decoding(peer, videoWidth, videoHeight);

      if (!open_codec(peer, inputFormat.getEncoding()))
          throw new ResourceUnavailableException("Couldn't open codec for " + inputFormat.toString());
    }

    public void close() {
      System.out.println("AVCODEC: close");
      close_codec( peer );
    }

    public void reset() {
      System.out.println("AVCODEC: reset");
      try {
          close();
          open();
      }
      catch(Exception e) {
          e.printStackTrace();
      }

    }

    public void finalize() {
        close();
    }


   public int process(Buffer inputBuffer, Buffer outputBuffer) {

      if (!checkInputBuffer(inputBuffer) ) {
         return BUFFER_PROCESSED_FAILED;
      }

      if (isEOM(inputBuffer) ) {
         propagateEOM(outputBuffer);
         return BUFFER_PROCESSED_OK;
      }

      int outMaxLength=131072;//outputFormat.getMaxDataLength();
      Object outDataTmp = getOutputData(outputBuffer);


      if ( outDataTmp == null || outputBuffer.getFormat() != outputFormat ||
           !outputBuffer.getFormat().equals(outputFormat) ) {
        outputBuffer.setLength(outMaxLength);
        outputBuffer.setFormat(outputFormat);
      }

      outDataTmp = validateData(outputBuffer, 0, true);
      boolean ret = decodeData(inputBuffer,outputBuffer);

      if (ret) {
           updateOutput(outputBuffer,outputFormat, outMaxLength, 0);
           return BUFFER_PROCESSED_OK;
       }
       else {
         if (DEBUG)
           System.out.println("[JavaDecoder] : returning OUTPUT_BUFFER_NOT_FILLED; ");
         return OUTPUT_BUFFER_NOT_FILLED;
       }


    }



    boolean decodeData(Buffer inputBuffer,Buffer outputBuffer) {

        int outMaxLength=131072;//outputFormat.getMaxDataLength();
        int inputLength = inputBuffer.getLength();
        //byte [] outData = (byte [])validateByteArraySize(outputBuffer,outMaxLength );//outputBuffer.getData();
        int [] outData = (int [])validateIntArraySize(outputBuffer,outMaxLength );
        long outDataBytes = getNativeData(outData);
        byte[] inputData = (byte [])inputBuffer.getData();
        long inDataBytes = getNativeData(inputData);


        if (inputLength <= 0) {
            return false;
        }

        boolean res = convert(peer, inputData, inDataBytes, inputBuffer.getOffset(), outData, outDataBytes, inputLength, inputBuffer.getTimeStamp()/1000000000.0);
        int size = lastAudioSize(peer);
        outputBuffer.setLength(size);
        return res;
    }

    public boolean checkFormat(Format format) {

        return super.checkFormat(format);
    }


}

