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
 * ChangeLog:
 *  Robert Hastings 2007/01/04
 *   - Native Library Location routines
 *   - Improvements to frame buffer management
 */
 
package com.omnividea.media.codec.video;
import com.moesol.bindings.NativeLibraryFinder;
import com.omnividea.FobsConfiguration;
import com.omnividea.media.parser.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import javax.media.*;
import javax.media.format.*;
import javax.media.format.*;
import com.sun.media.*;
import com.sun.media.controls.*;
import com.ibm.media.codec.video.*;
import java.awt.Dimension;
import javax.media.rtp.*;
import javax.media.control.*;
import java.awt.*;
import java.awt.event.*;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: </p>
 * @author not attributable
 * @version 1.0
 */

public class JavaDecoder extends VideoCodec {

  //Native Methods
  private native static boolean isBigEndian();
  private native void    init();
  private native boolean open_codec(long peer, String codec);
  private native boolean close_codec(long peer);
  private native void init_decoding(long peer, int width, int height);
  private native boolean convert( long peer,
                                  Object inData, long inDataBytes, int inOffset,
                                  Object outData,  long outDataBytes, long length, double dts);
  private long peer = 0;
  private boolean outputDone = false;


  // RGB bit masks
  static private int bMask = 0x000000ff;
  static private int gMask = 0x0000ff00;
  static private int rMask = 0x00ff0000;
  static final private boolean DEBUG = false;
  static private boolean bigEndian = true;

  private ByteBuffer byteBuffer = null;
  private int[] pixels;


  private int DEF_WIDTH = 352; 
  private int DEF_HEIGHT = 240;
  ////////////////////////////////////////////////////////////////////////////
  // Variables

  static {
    try {
      NativeLibraryFinder.loadLibrary(JavaDecoder.class, "fobs4jmf");
      System.out.println("Fobs4JMF - Native shared library found");
      bigEndian = isBigEndian();

      if(bigEndian == false)
      {
        System.out.println("Little Endian");
        //bMask = 0xff000000;
        //gMask = 0x00ff0000;
        //rMask = 0x0000ff00;
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
    supportedInputFormats = new VideoFormat[] {

        new VideoFormat("iv31"), //CODEC_ID_INDEO3
        new VideoFormat("iv32"),

        new VideoFormat("msvc"), //CODEC_ID_MSVIDEO1
        new VideoFormat("cram"),
        new VideoFormat("wham"),

        new VideoFormat("wmv1"), //CODEC_ID_WMV1

        new VideoFormat("wmv2"), //CODEC_ID_WMV2

        new VideoFormat("mpeg"), //CODEC_ID_MPEG1VIDEO
        new VideoFormat("mpg1"),
        new VideoFormat("mpg2"),
        new VideoFormat("pim1"),
        new VideoFormat("vcr2"),

        new VideoFormat("mjpa"), //CODEC_ID_MJPEG
        new VideoFormat("mjpb"),
        new VideoFormat("mjpg"),
        new VideoFormat("ljpg"),
        new VideoFormat("jpgl"),
        new VideoFormat("avdj"),

        new VideoFormat("svq1"), //CODEC_ID_SVQ1
        new VideoFormat("svqi"),

        new VideoFormat("svq3"), //CODEC_ID_SVQ3

        new VideoFormat("mp4v"), //CODEC_ID_MPEG4
        new VideoFormat("divx"),
        new VideoFormat("dx50"),
        new VideoFormat("xvid"),
        new VideoFormat("mp4s"),
        new VideoFormat("m4s2"),
        new VideoFormat("div1"),
        new VideoFormat("blz0"),
        new VideoFormat("ump4"),


        new VideoFormat("h264"), //CODEC_ID_H264

        new VideoFormat("h263"), //CODEC_ID_H263

        new VideoFormat("u263"), //CODEC_ID_H263P
        new VideoFormat("viv1"),

        new VideoFormat("i263"), //CODEC_ID_i263

        new VideoFormat("dvc"), //CODEC_ID_DVVIDEO
        new VideoFormat("dvcp"),
        new VideoFormat("dvsd"),
        new VideoFormat("dvhs"),
        new VideoFormat("dvs1"),
        new VideoFormat("dv25"),


        new VideoFormat("vp31"), //CODEC_ID_VP3

        new VideoFormat("rpza"), //CODEC_ID_RPZA

        new VideoFormat("cvid"), //CODEC_ID_CINEPAK

        new VideoFormat("smc"), //CODEC_ID_SMC


        new VideoFormat("mp42"), // CODEC_ID_MSMPEG4V2
        new VideoFormat("div2"),

        new VideoFormat("mpg4"), // CODEC_ID_MSMPEG4V1

        new VideoFormat("div3"), // CODEC_ID_MSMPEG4V3
        new VideoFormat("mp43"),
        new VideoFormat("mpg3"),
        new VideoFormat("div5"),
        new VideoFormat("div6"),
        new VideoFormat("div4"),
        new VideoFormat("ap41"),
        new VideoFormat("col1"),
        new VideoFormat("col0")

};
    defaultOutputFormats = new VideoFormat[] {
        new RGBFormat()};
    PLUGIN_NAME = "Fobs Decoder";

  }

  protected Format[] getMatchingOutputFormats(Format in) {
    System.out.println("AVCODEC: getMatchingOutputFormats");
    VideoFormat ivf = (VideoFormat) in;
    Dimension inSize = ivf.getSize();
    if (inSize == null) inSize = new Dimension(DEF_WIDTH, DEF_HEIGHT); 
    int maxDataLength = ivf.getMaxDataLength();


    supportedOutputFormats = new VideoFormat[] {
        new RGBFormat(new Dimension(inSize),
                      inSize.width * inSize.height, int[].class,
                      ivf.getFrameRate(),
                      32,
                      rMask, gMask, bMask,
                      1, inSize.width,
                      Format.FALSE, // flipped
                      Format.NOT_SPECIFIED// bigEndian?RGBFormat.BIG_ENDIAN:RGBFormat.LITTLE_ENDIAN // endian
                      )};
/*
    supportedOutputFormats = new VideoFormat[] {
        new RGBFormat(new Dimension(inSize),
                      inSize.width * inSize.height*3, Format.byteArray,
                      ivf.getFrameRate(),
                      24,
                      1, 2, 3,
                      3, inSize.width*3,
                      Format.FALSE, // flipped
                      Format.NOT_SPECIFIED // endian
                      )};
*/
    return supportedOutputFormats;
  }



  /**
   * Set the data input format.
   * @return false if the format is not supported.
   */
  public Format setInputFormat(Format format) {
    System.out.println("AVCODEC: setInputFormat");
    if (super.setInputFormat(format) != null) {
      //reset();
      VideoFormat ivf = (VideoFormat)format;
      Dimension ivfSize = ivf.getSize(); 
      if (ivfSize == null) ivfSize = new Dimension(DEF_WIDTH, DEF_HEIGHT); 
      int videoWidth = ivfSize.width;
      int videoHeight = ivfSize.height;

      outputFormat = new RGBFormat (new Dimension(videoWidth,videoHeight),
                                    videoWidth * videoHeight, int[].class,
                                    ivf.getFrameRate(),
                                    32,
                                    rMask, gMask, bMask,
                                    1,videoWidth,
                                    Format.FALSE, // flipped
                                    Format.NOT_SPECIFIED//bigEndian?RGBFormat.BIG_ENDIAN:RGBFormat.LITTLE_ENDIAN // endian
                                    );

      /*
      outputFormat = new RGBFormat (new Dimension(videoWidth,videoHeight),
                                    videoWidth * videoHeight*3, Format.byteArray,
                                    ivf.getFrameRate(),
                                    24,
                                    1, 2, 3,
                                    3,videoWidth*3,
                                    Format.FALSE, // flipped
                                    Format.NOT_SPECIFIED // endian
                                    );
      */
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

      System.out.println("AVCODEC: init_decoding");
      VideoFormat ivf = (VideoFormat)inputFormat;
      Dimension ivfSize = ivf.getSize(); 
      if (ivfSize == null) ivfSize = new Dimension(DEF_WIDTH, DEF_HEIGHT); 
      int videoWidth = ivfSize.width; //sdhays 
      int videoHeight = ivfSize.height; //sdhays 

      init_decoding(peer, videoWidth, videoHeight);

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

    // called when video resize is detected, by checkFormat()
    protected void videoResized() {
//        initDecoder();
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

      /*
      int outMaxLength=outputFormat.getMaxDataLength();
      Object outDataTmp = getOutputData(outputBuffer);
      if ( outDataTmp == null || outputBuffer.getFormat() != outputFormat ||
           !outputBuffer.getFormat().equals(outputFormat) ) {
        outputBuffer.setLength(outMaxLength);
        outputBuffer.setFormat(outputFormat);
      }

      outDataTmp = validateData(outputBuffer, 0, true);
      */

      
      if (outputBuffer.getFormat() != outputFormat ||
          !outputBuffer.getFormat().equals(outputFormat) ) 
      {
        outputBuffer.setFormat(outputFormat);
      }
      int []data;
      VideoFormat outFormat = (VideoFormat)outputBuffer.getFormat();
      int needDataSize = outFormat.getSize().width*outFormat.getSize().height;
      Object obj = outputBuffer.getData();
      
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
          outputBuffer.setData(data);
      } else {
          data = (int[]) obj;
      }*/
      
      if (dataNotOkay(obj, needDataSize)) {
    	  if (pixels == null) {
    		  pixels = new int[needDataSize];
    	  }
    	  data = pixels;
    	  outputBuffer.setData(data);
      } else {
    	  data = (int[]) obj;
      }

      boolean ret = decodeData(inputBuffer,outputBuffer);

      if (ret) {
           updateOutput(outputBuffer,outputFormat, outputFormat.getMaxDataLength(), 0);
           return BUFFER_PROCESSED_OK;
       }
       else {
         if (DEBUG)
           System.out.println("[JavaDecoder] : returning OUTPUT_BUFFER_NOT_FILLED; ");
         return OUTPUT_BUFFER_NOT_FILLED;
       }


    }

   private boolean dataNotOkay(Object obj, int needDataSize) {
		return (obj == null) ||
		      (! (obj instanceof int[]) ) ||
		      ( ((int[])obj).length < needDataSize);
   }

    boolean decodeData(Buffer inputBuffer,Buffer outputBuffer) 
    {

      boolean flag;
      
      Object outData = null;
      long outDataBytes = 0;
      VideoFormat outFormat = (VideoFormat)outputBuffer.getFormat();
      double dts = 1;
      
      int needDataSize = outFormat.getSize().width*outFormat.getSize().height;
      if(FobsConfiguration.useNativeBuffers)
      {
          if(byteBuffer == null)
          {
              byteBuffer = ByteBuffer.allocateDirect(4*needDataSize);
              if(isBigEndian()==false) byteBuffer.order(ByteOrder.LITTLE_ENDIAN);            
          }
          outData  = (Object)byteBuffer;
          dts = -1; //flag native
      }
      else
      {
        int outMaxLength=outputFormat.getMaxDataLength();
        outData = (Object)validateIntArraySize(outputBuffer,outMaxLength ); //int[]
        outDataBytes = getNativeData(outData);
      }

      int inputLength = inputBuffer.getLength();
      byte[] inputData = (byte [])inputBuffer.getData();
      long inDataBytes = getNativeData(inputData);


      if (inputLength <= 0) {
        return false;
      }

      boolean ret = convert(peer, inputData, inDataBytes, inputBuffer.getOffset(), outData, outDataBytes, inputLength, dts /*inputBuffer.getTimeStamp()/1000000000.0*/);
        
      if(ret && FobsConfiguration.useNativeBuffers){
         byteBuffer.asIntBuffer().get((int [])outputBuffer.getData());
      }
      return ret;
    }

    public boolean checkFormat(Format format) {

      if ( (format.getEncoding()).equals(VideoFormat.H263_RTP) ) {
        return true;
      }
      else {
        return super.checkFormat(format);
      }

    }


}

