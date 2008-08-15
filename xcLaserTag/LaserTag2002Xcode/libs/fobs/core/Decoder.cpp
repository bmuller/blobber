/******************************************************************************
* FOBS C++ wrapper code 
* Copyright (c) 2004 Omnividea Multimedia S.L
* Coded by José San Pedro Wandelmer
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


extern "C"
{
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavcodec/opt.h"
#include "libavutil/fifo.h"
#include "libavutil/avstring.h"
}

#include "Decoder.h"
#include <iostream>
#include "PacketBuffer.h"

using namespace std;
using namespace omnividea::fobs;

class _AVInit
{
   public:
   _AVInit()
   {
	  avcodec_register_all();
	  //avdevice_register_all();
      av_register_all();
   }
   ~_AVInit()
   {
      //av_free_static();
   }
};
static _AVInit __singleton;

static enum CodecID find_codec(const char *name, int type, int encoder)
{
    const char *codec_string = encoder ? "encoder" : "decoder";
    AVCodec *codec;
	
    if(!name)
        return CODEC_ID_NONE;
    codec = encoder ?
	avcodec_find_encoder_by_name(name) :
	avcodec_find_decoder_by_name(name);
    if(!codec) {
		return CODEC_ID_NONE;
    }
    if(codec->type != type) {
		return CODEC_ID_NONE;
    }
    return codec->id;
}

void Decoder::setAudioResampleFlag(bool flag)
{
	this->audioResampleFlag = flag;
}


FrameIndex Decoder::frameIndexFromTimeStamp(TimeStamp t)
{
    return (FrameIndex)(t / FOBS_TIMESTAMP_UNITS_D * getFrameRate());
}
TimeStamp Decoder::timeStampFromFrameIndex(FrameIndex f)
{
    return (TimeStamp)((double)f / (double)getFrameRate() * FOBS_TIMESTAMP_UNITS_D);
}

bool Decoder::compareTimeStamps(TimeStamp t1, TimeStamp t2)
{
    bool res =  false;
    if(isVideoPresent()) res = ::abs(t2-t1) <= 1000.0/getFrameRate();
    else res = ::abs(t2-t1) <= 1000.0/getAudioSampleRate();
    //cout << "T1: " << t1 << " == T2: " << t2 << " : " << res << endl;
    return res;
}

TimeStamp Decoder::pts2TimeStamp(int64_t pts, AVRational *pts_timebase) {
	//cout << "(Pts) " << pts << "* (Base Num)" << pts_timebase->num << "/ (double)(Base Den)"<<pts_timebase->den<<"*"<<FOBS_TIMESTAMP_UNITS_D<<endl;
	TimeStamp t= (TimeStamp)(pts*pts_timebase->num/(double)pts_timebase->den*FOBS_TIMESTAMP_UNITS_D);
	//cout << "Result: " << t <<endl;
	return t;
   }
int64_t Decoder::timeStamp2pts(TimeStamp ts, AVRational *pts_timebase) {
	return (int64_t)(ts * (double)pts_timebase->den / pts_timebase->num / FOBS_TIMESTAMP_UNITS_D);
   }

ReturnCode Decoder::testOpen()
{
   ReturnCode error = opened?OkCode:NotInitializedError;
   return error;
}
ReturnCode Decoder::testClose()
{
   ReturnCode error = (!opened)?OkCode:AlreadyInitializedError;
   return error;
}

Decoder::Decoder(const char *filename)
{
   strcpy(this->filename,filename);
   audioEnabledFlag = false;
   audioResampleFlag = false;
   incorrectPts = false;
   
   //derived from static members 
   yuvPicture = new AVPicture();
   rgbPicture = new AVPicture();
   rgbaPicture = new AVPicture();
   decodedPicture = new AVPicture();
   transitionPicture = new AVPicture();
   transitionPictureRgb = new AVPicture();   
   videoBuffer = new PacketBuffer();
   audioBuffer = new PacketBuffer();

    reset();
}
Decoder::~Decoder()
{
   close();
   
   delete yuvPicture;
   delete rgbPicture;
   delete rgbaPicture;
   delete decodedPicture;
   delete transitionPicture;
   delete transitionPictureRgb;
   delete videoBuffer;
   delete audioBuffer;
}

void Decoder::reset()
{
   rgbBuf = NULL;
   rgbaBuf = NULL;
   yuvBuf = NULL;
   yuvBytes = NULL;
   opened = false;

   img_resample_ctx = NULL;
   audioResampler = NULL;
   inputFile = NULL;
   inputFileFormat = NULL;
   transitionPictureBuf = NULL;
   transitionPictureBufRgb = NULL;
   currentYuv = currentRgb = currentRgba = 0;
   currentYuvFlag = false;
   currentRgbFlag = false;
   currentRgbaFlag = false;
   position = 0;
   positionAudio = 0;
   frameFlag = false;


   videoStreamIndex = -1; 
   audioStreamIndex = -1;
   
   videoBuffer->clear();
   audioBuffer->clear();


}
ReturnCode Decoder::close()
{
   ReturnCode error = testOpen();
   if(isOk(error))
   {
      av_free(rgbBuf);
      av_free(rgbaBuf);
      av_free(yuvBuf);
      delete []yuvBytes;
   
      if(videoStreamIndex >= 0 && inputFile->streams[videoStreamIndex] != NULL)
      {
        avcodec_close(inputFile->streams[videoStreamIndex]->codec);
         //av_free(inputStream.st);
      }
      if(audioStreamIndex >= 0 && inputFile->streams[audioStreamIndex] != NULL)
      {
        avcodec_close(inputFile->streams[audioStreamIndex]->codec);
      }
      av_close_input_file(inputFile);
      av_free(inputFileFormat);
      av_free(transitionPictureBuf);
      av_free(transitionPictureBufRgb);
      //if(img_resample_ctx)img_resample_close(img_resample_ctx);
      if(img_resample_ctx)sws_freeContext(img_resample_ctx);
      if(audioResampler)audio_resample_close(audioResampler);
      reset();
   }


   return error;
}


ReturnCode Decoder::_open()
{
	int ret;

	
	
   AVFormatParameters params, *ap = &params;
   ReturnCode error = testClose();

   if(isOk(error))
   {
	   /* get default parameters  */
	   int audio_sample_rate = 44100;
	   int audio_channels = 1;
	   AVRational frame_rate = (AVRational) {0,0};
	   int frame_width  = 0;
	   int frame_height = 0;
	   int frame_padtop  = 0;
	   int frame_padbottom = 0;
	   int frame_padleft  = 0;
	   int frame_padright = 0;
	   enum PixelFormat frame_pix_fmt = PIX_FMT_NONE;
	   int  video_channel = 0;
	   char  *audio_codec_name = NULL;
	   char  *video_codec_name = NULL;
	   
	   
	   inputFile = av_alloc_format_context();
	   
	   memset(ap, 0, sizeof(*ap));
	   ap->prealloced_context = 1;
	   ap->sample_rate = audio_sample_rate;
	   ap->channels = audio_channels;
	   ap->time_base.den = frame_rate.num;
	   ap->time_base.num = frame_rate.den;
	   ap->width = frame_width + frame_padleft + frame_padright;
	   ap->height = frame_height + frame_padtop + frame_padbottom;
	   ap->pix_fmt = frame_pix_fmt;
	   ap->channel = video_channel;
	   ap->video_codec_id = find_codec(video_codec_name, CODEC_TYPE_VIDEO, 0);
	   ap->audio_codec_id = find_codec(audio_codec_name, CODEC_TYPE_AUDIO, 0);

	   //AVFormatContext *avformat_opts = av_alloc_format_context();
	   //set_context_opts(inputFile, avformat_opts, AV_OPT_FLAG_DECODING_PARAM);
	   
	   inputFile->video_codec_id   = find_codec(video_codec_name   , CODEC_TYPE_VIDEO   , 0);
	   inputFile->audio_codec_id   = find_codec(audio_codec_name   , CODEC_TYPE_AUDIO   , 0);
	   
      /* open the input file with generic libav function */
      ret = av_open_input_file(&inputFile, filename, inputFileFormat, 0, ap);
      if (ret < 0) 
      {
         error = FileOpenError;
      }
   }
   if(isOk(error))
   {
      /* If not enough info to get the stream parameters, we decode the
       first frames to get it. (used in mpeg case for example) */
      ret = av_find_stream_info(inputFile);
      if (ret < 0) {
         error = FormatUnsupportedError;
      }
   }

   /*
   if(isOk(error))
   {
      dump_format(inputFile, 1, filename.c_str(), 0);
   }
   */
   
   if(isOk(error))
   {
      //eofReachedFlag = false;
      
      int discard = 1;
      //Get just two streams...First Video & First Audio
      for(int i=0; i < inputFile->nb_streams; i++) 
      {
         if(inputFile->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO && videoStreamIndex == -1)
         {
            discard = 0;
            videoStreamIndex = i;
         }
         else if(inputFile->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO && audioStreamIndex == -1)
         {
            discard = 0;
            audioStreamIndex = i;
         }

      }
      if(discard == 1)
      {
         error = VideoStreamMissingError;
      }
   }

   if(isOk(error))
   {
      //test initialization of variables
      if(isVideoPresent() && (_getWidth()==0 || _getHeight() == 0))
      {
        error = FormatUnsupportedError;
      }
   }

	if(isOk(error) && isAudioPresent())
	{
		//cout << "Audio Resample Flag: "<<audioResampleFlag<<endl;
		if(inputFile->streams[audioStreamIndex]->codec->channels > 2 && audioResampleFlag)
		{
			inputFile->streams[audioStreamIndex]->codec->channels = 2;
			inputFile->streams[audioStreamIndex]->codec->request_channels=2;
			//audioResampler = audio_resample_init(2, inputFile->streams[audioStreamIndex]->codec->channels, getAudioSampleRate(), getAudioSampleRate());
			//cout << "AudioResampler will be needed: " << audioResampler << endl;
		}
	}
	
   if(isOk(error))
   {
     AVCodec *codec;
     if(videoStreamIndex >= 0)
     {
       codec = avcodec_find_decoder(inputFile->streams[videoStreamIndex]->codec->codec_id);
       if (!codec) 
       {
           error = CodecUnsupportedError;
       }
       else if (avcodec_open(inputFile->streams[videoStreamIndex]->codec, codec) < 0) 
       {
           error = CodecUnsupportedError;
       }
     }
     if(audioStreamIndex >= 0)
     {
       codec = avcodec_find_decoder(inputFile->streams[audioStreamIndex]->codec->codec_id);
       if (!codec) 
       {
           //error = CodecUnsupportedError;
           audioStreamIndex = -1;
           cout << "Audio codec unsupported!\n";
       }
       else if (avcodec_open(inputFile->streams[audioStreamIndex]->codec, codec) < 0) 
       {
           //error = CodecUnsupportedError;
           audioStreamIndex = -1;
           cout << "Audio codec unsupported!\n";
       }
     }
   }

   if(isOk(error) && isVideoPresent())
   {
      yuvBytes = new char[_getWidth()*_getHeight()];
      if(yuvBytes == NULL)
      {
         error = MemoryError;
      }
   }
   if(isOk(error))
   {
	   if(firstVideoPositionFlag == false)
	   {
		   firstVideoPosition = 0;
		   firstVideoFramePosition = -1;
	   }
   	   if(firstAudioPositionFlag == false)
	   {
		   firstAudioPosition = 0;
		   firstAudioFramePosition = -1;
	   }
      opened = true;
   }
   
   return error;
}
ReturnCode Decoder::open()
{
   ReturnCode error = testClose();
   if(isError(error)) return error;
   
   reset();
   firstVideoPositionFlag = false;
   firstAudioPositionFlag = false;
  
   error = _open();

   //find first video frame
   if(isVideoPresent())
   {
       while((isOk(error)|| error == NoFrameError) && firstVideoFramePosition == -1) error = nextFrame();
   }
   else
   {
       while((isOk(error)|| error == NoFrameError) && firstAudioFramePosition == -1) error = nextAudioFrame();
   }

   if(isOk(error))
   {
      if(inputFile->duration == (TimeStamp)AV_NOPTS_VALUE) 
      {
        if(isVideoPresent())
        {
            while(placeAtNextFrame(true) == 0)
            {
              videoBuffer->deleteNext();
              duration =  position;
            }
            if(duration == 0)
            {
               error = FormatUnsupportedError;
            }
        }
        else
        {
            while(placeAtNextFrame(false) == 0)
            {
              audioBuffer->deleteNext();
              duration =  positionAudio;
            }
            if(duration == 0)
            {
               error = FormatUnsupportedError;
            }            
        }
      }
      else
      {
          cout << ((double)inputFile->duration/(double)AV_TIME_BASE);
          duration = (TimeStamp) ((double)inputFile->duration*FOBS_TIMESTAMP_UNITS_D/(double)AV_TIME_BASE);
      }


      cout << "First Position: " << firstVideoPosition << ", " << firstVideoFramePosition << " Duration: " << duration << endl;
      if(isVideoPresent())cout << "Frame Rate: " << getFrameRate() << endl;
   }

   close();
   
   if(isOk(error))
   {
      return _open();
   }
      
   return error;
}

double Decoder::getFirstFrameTime()
{
  return firstVideoFramePosition / FOBS_TIMESTAMP_UNITS_D;
}
double Decoder::getFirstAudioSampleTime()
{
  return firstAudioFramePosition / FOBS_TIMESTAMP_UNITS_D;
}

omnividea::fobs::uint Decoder::getWidth()
{
   ReturnCode error = testOpen();
   if(isOk(error))
   {
      return _getWidth();
   }
   else
   {
      return 0;
   }
}
omnividea::fobs::uint Decoder::_getWidth()
{
   return inputFile->streams[videoStreamIndex]->codec->width;
}
omnividea::fobs::uint Decoder::getHeight()
{
   ReturnCode error = testOpen();
   if(isOk(error))
   {
      return _getHeight();
   }
   else
   {
      return 0;
   }
}
omnividea::fobs::uint Decoder::_getHeight()
{
   return inputFile->streams[videoStreamIndex]->codec->height;
}
int Decoder::getBitRate()
{
   ReturnCode error = testOpen();
   if(isOk(error))
   {
      int res = inputFile->streams[videoStreamIndex]->codec->bit_rate / 1000;
      if(res == 0) res = inputFile->bit_rate / 1000;
      return res;
   }
   else
   {
      return 0;
   }
}
double Decoder::getFrameRate()
{
   ReturnCode error = testOpen();
   if(isOk(error))
   {
      //return (double)inputStream.st->codec.frame_rate / DEFAULT_FRAME_RATE_BASE; //For ffmpeg-0.4.6
      //cout << "FR: " << inputFile->streams[videoStreamIndex]->codec.frame_rate << " - Base: " << inputFile->streams[videoStreamIndex]->codec.frame_rate_base << endl;
      
      //return (double)inputFile->streams[videoStreamIndex]->codec->time_base.den / (double)inputFile->streams[videoStreamIndex]->codec->time_base.num; //For ffmpeg-0.4.8
      return (double)inputFile->streams[videoStreamIndex]->r_frame_rate.num / (double)inputFile->streams[videoStreamIndex]->r_frame_rate.den;
   }
   else
   {
      return -1;
   }
}
TimeStamp Decoder::getDurationMilliseconds()
{
   return duration;
}
double Decoder::getDurationSeconds()
{
   return duration/FOBS_TIMESTAMP_UNITS_D;
}

byte Decoder::getCrFactor()
{
   return 2;
}
byte Decoder::getCbFactor()
{
   return 2;
}

byte *Decoder::getLuminance()
{
   if(testOpen() != 0) return NULL;
   int ret;
   AVPicture *tmpPicture;
   enum PixelFormat pix_fmt=PIX_FMT_YUV420P;
   AVCodecContext *dec = inputFile->streams[videoStreamIndex]->codec;

   if(currentYuvFlag && compareTimeStamps(position, currentYuv))
   {
      return (byte *)yuvBytes;
   }
   
   /* convert pixel format if needed */
   if(pix_fmt == dec->pix_fmt) 
   {
      tmpPicture = decodedPicture;
   }
   else
   {
      /* create temporary picture */
      if(yuvPicture->data[0] == NULL)
      {
         int size = avpicture_get_size(pix_fmt, dec->width, dec->height);
         yuvBuf = (uint8_t *)av_malloc(size);
         if (!yuvBuf)
         {
            return NULL;
         }
         avpicture_fill(yuvPicture, yuvBuf, pix_fmt, dec->width, dec->height);
      }
     
      ret = img_convert(yuvPicture, pix_fmt, 
                       decodedPicture, dec->pix_fmt, 
                       dec->width, dec->height);
      if(ret < 0) 
      {
         return NULL;
      }
      tmpPicture = yuvPicture;
   }

   unsigned long offset = 0, srcOffset = 0;
   for(unsigned i = 0; i < _getHeight(); i++)
   {
      memcpy(yuvBytes+offset, tmpPicture->data[0]+srcOffset, getWidth());
      offset+=getWidth();
      srcOffset += decodedPicture->linesize[0];
   }
   currentYuv = position;
   currentYuvFlag = true;
   return (byte *)yuvBytes;
}

byte *Decoder::getCr()
{
   if(testOpen() != 0) return NULL;
   AVCodecContext *dec = inputFile->streams[videoStreamIndex]->codec;
   enum PixelFormat pix_fmt=PIX_FMT_YUV420P;
   getLuminance();
   if(pix_fmt == dec->pix_fmt) 
   {
      return (byte *)(decodedPicture->data[1]);
   }
   else
   {
      return (byte *)(yuvPicture->data[1]);
   }
}
byte *Decoder::getCb()
{
   if(testOpen() != 0) return NULL;
   AVCodecContext *dec = inputFile->streams[videoStreamIndex]->codec;
   enum PixelFormat pix_fmt=PIX_FMT_YUV420P;
   getLuminance();
   if(pix_fmt == dec->pix_fmt) 
   {
      return (byte *)(decodedPicture->data[2]);
   }
   else
   {
      return (byte *)(yuvPicture->data[2]);
   }
}

ReturnCode Decoder::reallocTransitionPicture(int newWidth, int newHeight)
{
   ReturnCode error = OkCode;
   AVCodecContext *dec = inputFile->streams[videoStreamIndex]->codec;
   enum PixelFormat pix_fmt = dec->pix_fmt;
   if(transitionPictureBuf == NULL || 
      transitionPictureWidth != newWidth || 
      transitionPictureHeight != newHeight)
   {
      av_free(transitionPictureBuf);
      av_free(transitionPictureBufRgb);
      transitionPictureBuf = NULL;
      transitionPictureBufRgb = NULL;
      if(img_resample_ctx) sws_freeContext(img_resample_ctx);
      //if(img_resample_ctx)img_resample_close(img_resample_ctx);
      img_resample_ctx = NULL;
      int size = avpicture_get_size(pix_fmt, newWidth, newHeight);
      int sizeRgb = avpicture_get_size(PIX_FMT_RGB24, newWidth, newHeight);
      //cerr << "Decoder Transition: size=" << size << " sizeRgb=" << sizeRgb << " W=" << newWidth << " H=" << newHeight << endl;
      transitionPictureBuf = (uint8_t*)av_malloc(size);
      if (!transitionPictureBuf)
      {
         error = MemoryError;
      }
      if(isOk(error))
      {
         transitionPictureBufRgb = (uint8_t*)av_malloc(sizeRgb);
         if (!transitionPictureBufRgb)
         {
            error = MemoryError;
         }
      }
      if(isOk(error))
      {
         enum PixelFormat pix_fmt;
         AVPicture *pict;
         error = getRawFrame(&pict, (int *)&pix_fmt);
         if(isOk(error))
         {
            img_resample_ctx = sws_getContext(
                            this->_getWidth(), this->_getHeight(), pix_fmt,
                            newWidth, newHeight, pix_fmt,
                            0, NULL, NULL, NULL);
            //img_resample_ctx = img_resample_init( newWidth, newHeight, this->_getWidth(), this->_getHeight());
            if(img_resample_ctx == NULL)
            {
                error = MemoryError;
            }
        }
      }
      if(isOk(error))
      {
         avpicture_fill(transitionPicture, transitionPictureBuf, pix_fmt, newWidth, newHeight);
         avpicture_fill(transitionPictureRgb, transitionPictureBufRgb, PIX_FMT_RGB24, newWidth, newHeight);
         transitionPictureWidth = newWidth;
         transitionPictureHeight = newHeight;
      }
   }
   return error;
}

byte *Decoder::getRGB(int width, int height)
{
   AVPicture *pict;
   enum PixelFormat pix_fmt;
   ReturnCode error = getRawFrame(&pict, (int *)&pix_fmt);
   if(isError(error)) return NULL;

   enum PixelFormat dst_pix_fmt=PIX_FMT_RGB24;

   if(reallocTransitionPicture(width, height) == 0)
   {
      //img_resample(img_resample_ctx, transitionPicture, decodedPicture);
      sws_scale(img_resample_ctx, decodedPicture->data, decodedPicture->linesize, 0, transitionPictureHeight/*?*/, transitionPicture->data, transitionPicture->linesize);
      if(img_convert(transitionPictureRgb, dst_pix_fmt,transitionPicture, pix_fmt, width, height) < 0)
      {
         return NULL;
      }
   }
   else
   {
      return NULL;
   }
   return (byte *)transitionPictureRgb->data[0];
}

byte *Decoder::getRGB()
{
   if(testOpen() != 0) return NULL;
   int ret;
   AVPicture *tmpPicture;
   enum PixelFormat pix_fmt=PIX_FMT_RGB24;
   AVCodecContext *dec = inputFile->streams[videoStreamIndex]->codec;
   
   if(currentRgbFlag && compareTimeStamps(position, currentRgb))
   {
      if(pix_fmt == dec->pix_fmt) 
      {
         return (byte *)decodedPicture->data[0];
      }
      else
      {
         return (byte *)rgbPicture->data[0];
      }
   }


   /* convert pixel format if needed */
   if(pix_fmt == dec->pix_fmt) 
   {
      tmpPicture = decodedPicture;
   }
   else
   {
      /* create temporary picture */
      if(rgbBuf == NULL)
      {
         int size = avpicture_get_size(pix_fmt, dec->width, dec->height);
         rgbBuf = (uint8_t *)av_malloc(size);
         if (!rgbBuf)
         {
            return NULL;
         }
         avpicture_fill(rgbPicture, rgbBuf, pix_fmt, dec->width, dec->height);
      }
     
      ret = img_convert(rgbPicture, pix_fmt, 
                       decodedPicture, dec->pix_fmt, 
                       dec->width, dec->height);
      if(ret < 0) 
      {
         return NULL;
      }
      tmpPicture = rgbPicture;
   }
   currentRgb = position;
   currentRgbFlag = true;
   return (byte *)tmpPicture->data[0];
}

byte *Decoder::getRGBA()
{
   if(testOpen() != 0) return NULL;
   int ret;
   AVPicture *tmpPicture;
   enum PixelFormat pix_fmt=PIX_FMT_RGBA32;
   AVCodecContext *dec = inputFile->streams[videoStreamIndex]->codec;
   
   if(currentRgbaFlag && compareTimeStamps(position, currentRgba))
   {
      if(pix_fmt == dec->pix_fmt) 
      {
         return (byte *)decodedPicture->data[0];
      }
      else
      {
         return (byte *)rgbaPicture->data[0];
      }
   }


   /* convert pixel format if needed */
   if(pix_fmt == dec->pix_fmt) 
   {
      tmpPicture = decodedPicture;
   }
   else
   {
      /* create temporary picture */
      if(rgbaBuf == NULL)
      {
         int size = avpicture_get_size(pix_fmt, dec->width, dec->height);
         rgbaBuf = (uint8_t *)av_malloc(size);
         if (!rgbaBuf)
         {
            return NULL;
         }
         avpicture_fill(rgbaPicture, rgbaBuf, pix_fmt, dec->width, dec->height);
      }
     
      ret = img_convert(rgbaPicture, pix_fmt, 
                       decodedPicture, dec->pix_fmt, 
                       dec->width, dec->height);
      if(ret < 0) 
      {
         return NULL;
      }
      tmpPicture = rgbaPicture;
   }
   currentRgba = position;
   currentRgbaFlag = true;
   return (byte *)tmpPicture->data[0];
}

byte *Decoder::getRGBA(char *buf)
{
   if(testOpen() != 0) return NULL;
   int ret;
   enum PixelFormat pix_fmt=PIX_FMT_RGBA32;
   AVCodecContext *dec = inputFile->streams[videoStreamIndex]->codec;
   
   {
      AVPicture tmpPicture;
      avpicture_fill(&tmpPicture, (uint8_t *)buf, pix_fmt, dec->width, dec->height);    
      ret = img_convert(&tmpPicture, pix_fmt, 
                       decodedPicture, dec->pix_fmt, 
                       dec->width, dec->height);
      if(ret < 0) 
      {
         return NULL;
      }
   }
   return (byte *)buf;
}

ReturnCode Decoder::getRawFrame(AVPicture** pict, int* pix_fmt)
{
   ReturnCode error = testOpen();
   if(isOk(error))
   {
      AVCodecContext *dec = inputFile->streams[videoStreamIndex]->codec;
      *pix_fmt = (int)dec->pix_fmt;
      *pict = decodedPicture;
   }

   return error;
}

uint8_t* Decoder::getAudioSamples()
{
  //cout << "Requesting audio frame. Size="<<decodedAudioFrameSize;
  //cout << " which should be about "<<(double)decodedAudioFrameSize/getAudioChannelNumber()/2.0/getAudioSampleRate()*1000.0<<"ms"<<endl;
	return decodedAudioFrame;
}
int Decoder::getAudioSamplesSize()
{
  return decodedAudioFrameSize;
}

char* Decoder::getFileName()
{
   return filename;
}

/*bool Decoder::moreFrames()
{
   return !(eofReachedFlag);
}*/


FrameIndex Decoder::getFrameIndex()
{
    return frameIndexFromTimeStamp(position);
   //return (FrameIndex)((position - firstVideoFramePosition) * getFrameRate() + 0.5);
}


double Decoder::getFrameTime()
{
   return (position - firstVideoFramePosition)/FOBS_TIMESTAMP_UNITS_D;
}

double Decoder::getNextFrameTime()
{
   return getFrameTime() + 1.0/getFrameRate();
}

ReturnCode Decoder::readNextFrame()
{
   static unsigned counter = 0;
   AVPacket pkt;
   int frameReady = false;
   ReturnCode error = testOpen();
   /*if(isOk(error))
   {
      if(eofReachedFlag) error = VideoEndError;
   }*/
   while(isOk(error) && !frameReady)
   {
     /* read a packet from it and output it in the fifo */
     if (av_read_frame(inputFile, &pkt) < 0) 
     {
        //eofReachedFlag = true;
        error = VideoEndError;
        continue;
     }

     if(pkt.stream_index == videoStreamIndex)
     {
       videoBuffer->append(&pkt);
     }
     else if(pkt.stream_index == audioStreamIndex)
     {
        audioBuffer->append(&pkt);
     }
     else
     {
        av_free_packet(&pkt);
        continue;
     }

     frameReady = true;
  }
  return error;
}


ReturnCode Decoder::placeAtNextFrame(bool videoFrame)
{
   ReturnCode error = testOpen();
   PacketBuffer *buffer = videoFrame?videoBuffer:audioBuffer;
   /*if(isOk(error))
   {
      if(eofReachedFlag) error = VideoEndError;
   }*/
   while(isOk(error) && buffer->count() <= 0)
   {
     error = readNextFrame();
   }
   if(isOk(error))
   {
     AVPacket *pkt = buffer->readNext();
     //if(pkt != NULL) cout << "-DTS " << (videoFrame?"Video":"Audio") << ": "<< pkt->dts << " size:" << pkt->size << endl;
     if(pkt == NULL)
     {
        error = VideoPacketBufferEmptyError;
     }
     else if(pkt->size > 0) 
     {
        if(videoFrame)
        {
            AVRational vTimeBase = inputFile->streams[videoStreamIndex]->time_base;

            if(pkt->dts == (TimeStamp)AV_NOPTS_VALUE)
            {
               position += (TimeStamp)(FOBS_TIMESTAMP_UNITS_D/getFrameRate());
			   //cout << "PosNew=" << position <<endl;
               if(firstVideoPositionFlag == false)
               {
                firstVideoPosition = position;
                firstVideoPositionFlag = true;
               }
               else
               {
                 if(position < firstVideoPosition) firstVideoPosition = position;
               }
            }
            else
            {
               //position += (TimeStamp)(FOBS_TIMESTAMP_UNITS_D/getFrameRate());
			   
               if(firstVideoPositionFlag == false || ( pts2TimeStamp(pkt->dts, &vTimeBase) < firstVideoPosition ) )
               {
                firstVideoPosition = pts2TimeStamp(pkt->dts, &vTimeBase);
                firstVideoPositionFlag = true;
               }
               position = pts2TimeStamp(pkt->dts - timeStamp2pts(firstVideoPosition, &vTimeBase), &vTimeBase);
               //cout << "VIDEO PosNew=" << position << "   DTS="<<pkt->dts<<"   PTS=" << pkt->pts << "  DEN=" << vTimeBase.den << "   NUM=" << vTimeBase.num << " FVideoDTS	: " << timeStamp2pts(firstVideoPosition, &vTimeBase) << endl;
			   AVStream *s = (inputFile->streams[videoStreamIndex]);
            }
            frameFlag = true;
	    }
        else
        {
            AVRational aTimeBase = inputFile->streams[audioStreamIndex]->time_base;
            int aDen = aTimeBase.den;

            if((videoStreamIndex < 0 && firstAudioPositionFlag != -1 && pkt->dts == 0 && pkt->pts == 0)||incorrectPts)
            {
                pkt->dts = (TimeStamp)AV_NOPTS_VALUE;
                incorrectPts = true;
                //cout << "Incorrect Time Values detected\n";
            }
            if(pkt->dts == (TimeStamp)AV_NOPTS_VALUE)
            {
                positionAudio += (TimeStamp)((double)FOBS_TIMESTAMP_UNITS_D*(double)getAudioSamplesSize()/getAudioChannelNumber()/2/getAudioSampleRate());
                //cout << "FiguringOut Audio Position: "<< positionAudio << endl;
               if(firstAudioPositionFlag == false)
               {
                firstAudioPosition = position;
                firstAudioPositionFlag = true;
               }
               else
               {
                 if(positionAudio < firstAudioPosition) firstAudioPosition = positionAudio;
               }
            }
            else
            {
               //cout << "DTS" << pkt->pts <<endl;
               if(firstAudioPositionFlag == false || pts2TimeStamp(pkt->dts, &aTimeBase) < firstAudioPosition )
               {
				   //cout << "Changing first audio" << endl;
			    firstAudioPosition = pts2TimeStamp(pkt->dts, &aTimeBase);
                firstAudioPositionFlag = true;
               }
               positionAudio = pts2TimeStamp(pkt->dts - timeStamp2pts(firstAudioPosition, &aTimeBase), &aTimeBase);
            }
               //cout << "AUDIO PosNew=" << positionAudio << "   DTS="<<pkt->dts<<"   PTS=" << pkt->pts << "  DEN=" << aTimeBase.den << "   NUM=" << aTimeBase.num << " FAudioDTS	: " << timeStamp2pts(firstAudioPosition, &aTimeBase)<<endl;
        }
     }
     else
     {
        frameFlag = false;
     }
   }
     /*
     cout << "PTS: ";
     if(pkt.pts == AV_NOPTS_VALUE) cout << "UNKNOWN ";
     else cout << ((double)pkt.pts/(double)AV_TIME_BASE);
     cout << endl;
     
     cout << "DTS: ";
     if(pkt.dts == AV_NOPTS_VALUE) cout << "UNKNOWN ";
     else cout << ((double)pkt.dts/(double)AV_TIME_BASE);
     cout << endl;
     */
   
   return error;
}

ReturnCode Decoder::decodeFrame()
{
   ReturnCode error = testOpen();
   int got_picture = 0;
   AVPacket *pkt = videoBuffer->extractNext();
   if(pkt == NULL)
   {
     error = VideoPacketBufferEmptyError;
   }
   if(isOk(error))
   {
      if(pkt->size == 0) 
      {
        error = BadParamsError;
      }
   }
   if(isOk(error))
   {
    //cout << "Decode Video: " << pkt->dts << endl;
    if (inputFile->streams[videoStreamIndex]->codec->codec_id == CODEC_ID_RAWVIDEO) 
    {
       int size;
       size = (_getWidth() * _getHeight());
       avpicture_fill(decodedPicture, pkt->data, 
                    inputFile->streams[videoStreamIndex]->codec->pix_fmt,
                    inputFile->streams[videoStreamIndex]->codec->width,
                    inputFile->streams[videoStreamIndex]->codec->height);
    } 
    else 
    {
       AVFrame big_picture;
    
       int ret = avcodec_decode_video(inputFile->streams[videoStreamIndex]->codec, 
                                  &big_picture, &got_picture, pkt->data, pkt->size);

       *decodedPicture= *(AVPicture*)&big_picture;
       inputFile->streams[videoStreamIndex]->quality= big_picture.quality;
       if (ret < 0 || !got_picture) 
       {
          error = NoFrameError;
       }
       else if(firstVideoFramePosition == -1)
       {
        firstVideoFramePosition = position;
       }
    }
    av_free_packet(pkt);
  }
  return error;
}

ReturnCode Decoder::decodeAudioFrame()
{
   ReturnCode error = testOpen();
   int data_size = 0;
   //short samples[AVCODEC_MAX_AUDIO_FRAME_SIZE / 2];
   uint8_t *samples = decodedAudioFrame;
   
   AVPacket *pkt = audioBuffer->extractNext();


   if(pkt == NULL)
   {
     error = VideoPacketBufferEmptyError;
   }
   if(isOk(error))
   {
      if(pkt->size == 0) 
      {
        error = BadParamsError;
        av_free_packet(pkt);
      }
   }
   uint64_t ts;
   int ret;
   uint8_t *ptr;
   int len;
   if(isOk(error))
   {
    ts = pkt->dts;
    ptr = pkt->data;
    len = pkt->size;

    ret = avcodec_decode_audio(inputFile->streams[audioStreamIndex]->codec, (short*) samples, &data_size, ptr, len);
    
    if(ret < 0)
    {
       error = GenericError;
    }
    else if(ret == len)
    {
        av_free_packet(pkt);
    }
   }
   if(isOk(error))
   {
    /* Some bug in mpeg audio decoder gives */
    /* data_size < 0, it seems they are overflows */

    if (data_size <= 0) {
        //no audio frame
        decodedAudioFrameSize = 0;
        //cout << "No Audio Frame!!" << endl;
    }
    else
    {
        /* Only used if setAudioResampleFlag 
        if(audioResampler)
        {
            //Resampling needed
            int nSamples = data_size / _getAudioChannelNumber()/2;
            audio_resample(audioResampler, (short *)resampledAudioBuffer, (short *)samples, nSamples);
            data_size = nSamples * getAudioChannelNumber() * 2;
            //memcpy(samples, resampled, data_size);
        } */
      //cout << "Audio Decoded - DTS="<<((double)ts/(double)AV_TIME_BASE)<<" Size=" << data_size << " Max=" << AVCODEC_MAX_AUDIO_FRAME_SIZE << endl;
      //audioTime = ((double)ts/(double)AV_TIME_BASE);
      //memcpy(decodedAudioFrame, samples, data_size);
      if(firstAudioFramePosition == -1)
      {
        firstAudioFramePosition = positionAudio;
      }
      decodedAudioFrameSize = data_size;
      //cout << "OK!!" << endl;
	  //cout << "Audio decode ret=" << ret<< " len="<<len<<" data_size="<<data_size<<endl;
      if(ret != len)
      {
          //Some pkt data was not processed... include it again in the packet list
          //cout << "Ret="<<ret<<" - Len="<<len<<endl;
          if(pkt->size > 0) //Strange audio processing behaviour...
          {
              if(ret == 0)
              {
                  audioBuffer->prepend(pkt);
              }
              else
              {
                  AVPacket newPkt;
                  av_new_packet(&newPkt, pkt->size - ret);
                  memcpy(newPkt.data, pkt->data + ret, pkt->size - ret);
                  av_free_packet(pkt);
                  audioBuffer->prepend(&newPkt);
              }
         }
      }

    }
   }
   return error;
}

ReturnCode Decoder::nextFrame()
{
   //cout << "Next Video Frame Time: " << getNextFrameTime() << endl;
   ReturnCode error = placeAtNextFrame(true);
   
   if(isOk(error))
   {
      AVPacket *pkt = videoBuffer->readNext();
      if(pkt == NULL)
      {
        error = VideoPacketBufferEmptyError;
      }
      else if(pkt->size == 0) 
      {
        videoBuffer->deleteNext();
        return nextFrame();
      }
      else 
      {
        error = decodeFrame();
      }
   }

    if(error == NoFrameError) return nextFrame();
    return error;
}

ReturnCode Decoder::nextAudioFrame()
{

   ReturnCode error = placeAtNextFrame(false);
   if(isOk(error))
   {
      AVPacket *pkt = audioBuffer->readNext();
      if(pkt == NULL)
      {
        error = VideoPacketBufferEmptyError;
      }
      else if(pkt->size == 0)
      {
        audioBuffer->deleteNext();
        return nextAudioFrame();
      }
      else 
      {
        //cout << "Decoder AUDIO Time: " << pkt->dts << endl;
        
        error = decodeAudioFrame();
        if(isOk(error))
        {
		  //cout << "New Audio Sample Set" << endl;
          if(getAudioSamplesSize() <= 0)
          {
              return nextAudioFrame();
          }
        }
      }
   }

   return error;
}

ReturnCode Decoder::prevFrame()
{
   return setFrame(frameIndexFromTimeStamp(position) - 1);
}


TimeStamp Decoder::getAVPosition()
{
    if(isVideoPresent())
    {
        return position;
    }
    if(isAudioPresent())
    {
        return positionAudio;
    }
}

ReturnCode Decoder::_setFrame(TimeStamp newPosition)
{
    ReturnCode error = testOpen();
    if(isOk(error))
    {
      if(isVideoPresent() && firstVideoFramePosition == -1)
      {
        error = nextFrame();
          error = nextAudioFrame();		  
      }
      else if(isAudioPresent() && firstAudioFramePosition == -1)
      {
          error = nextFrame();
          error = nextAudioFrame();
      }
    }
    if(isOk(error))
    {
       if(newPosition > getDurationMilliseconds()) error = BadParamsError;
    }
    
    TimeStamp currentPosition=isVideoPresent()?position:positionAudio;
    TimeStamp firstPosition=isVideoPresent()?firstVideoFramePosition:firstAudioFramePosition;
    if(isOk(error) && !compareTimeStamps(currentPosition, newPosition))
    {
       videoBuffer->clear();
       audioBuffer->clear();
       TimeStamp newPos = newPosition - 1000;
       if(newPos < firstPosition) newPos = 0;
       if(newPosition < firstPosition) newPosition = firstPosition;
       int64_t realPos = newPos * 1000;
       cout << "Seeking pos: " << newPosition << " - Real: " << realPos << endl;
       
       int res = -1;
       
       if(!incorrectPts)
       {
           res = av_seek_frame(inputFile, -1, realPos, AVSEEK_FLAG_BACKWARD);
       }
       if(isVideoPresent())
       {
          if(res < 0)
          {
              error = setFrameClassic(newPosition);
          }
          else
          {
              error = setFrameFast(newPosition);
          }
       }
       else
       {
           cout << "Performing Audio seek: " << res << endl;
           if(res < 0)
           {
               error = setAudioClassic(newPosition);
           }
           else
           {
               error = setAudioFast(newPosition);
           }
       }
       audioBuffer->clear();
    }
    return error;
}

/*ReturnCode Decoder::setAudioFast(TimeStamp newPosition)
{
   ReturnCode error = OkCode;
   for(error = nextAudioFrame(); isOk(error);error = nextAudioFrame())
   {
      if(compareTimeStamps(positionAudio, newPosition)) break;
      if(positionAudio > newPosition) break;
   }
   return error;
}*/
ReturnCode Decoder::setAudioFast(TimeStamp newPosition)
{
   ReturnCode error = OkCode;
   for(error = placeAtNextFrame(false); isOk(error);error = placeAtNextFrame(false))
   {
      if(compareTimeStamps(positionAudio, newPosition)) break;
      if(positionAudio > newPosition) break;
      AVPacket *pkt = audioBuffer->extractNext();
      av_free_packet(pkt);
   }
   return error;
}
ReturnCode Decoder::setAudioClassic(TimeStamp newPosition)
{
   ReturnCode error = testOpen();
   cout << "SetAudioClassic Called... Use another format!\n";

   if(isOk(error))
   {
      if(newPosition > duration) error = BadParamsError;
   }
   if(isOk(error))
   {
      if(compareTimeStamps(positionAudio, newPosition))
      {
         error = OkCode;
      }
      else if(newPosition > position)
      {
          setAudioFast(newPosition);
      }
      else
      {
         error = close();
         if(isOk(error))
         {
            error = _open();
         }
         if(isOk(error))
         {
            error = setFrameByTime(newPosition);
         }
      }

   }

   return error;
}



ReturnCode Decoder::setFrameFast(TimeStamp newPosition)
{
   ReturnCode error = OkCode;
   bool keyFrame = false;
   for(error = placeAtNextFrame(true); isOk(error);)
   {
      AVPacket *pkt = videoBuffer->readNext();
      if(keyFrame == false && pkt->flags & PKT_FLAG_KEY)
      {
         keyFrame = true;
         error = decodeFrame();
      }
      else
      {
         if(keyFrame) error = nextFrame();
         else 
         {
            AVPacket *pkt = videoBuffer->extractNext();
            av_free_packet(pkt);
            error = placeAtNextFrame(true);
          }
      }

      //cout << "P: " << position << endl;
      if(compareTimeStamps(position, newPosition) || newPosition <= position) break;
   }
   
   //Workaround to sync a/v after a seek op
	//audioBuffer->clear();
	positionAudio = firstAudioPosition;
   while(audioBuffer->count()>0 && position>positionAudio && isOk(error))
   {
	   error = placeAtNextFrame(false);
	   error = decodeAudioFrame();
	   /*
    AVPacket *pkt = audioBuffer->readNext();
    AVRational aTimeBase = inputFile->streams[audioStreamIndex]->time_base;
    if(position +1000 < pts2TimeStamp(pkt->dts, &aTimeBase)) break;
    pkt = audioBuffer->extractNext();
    cout << "Extracting one audio packet to sync - VidPos: "<<position<<" and packet was: "<< pts2TimeStamp(pkt->pts, &aTimeBase) << endl;
    av_free_packet(pkt);
		*/
   }
   
   return error;
}

ReturnCode Decoder::setFrameClassic(TimeStamp newPosition)
{
   ReturnCode error = testOpen();
   cout << "SetFrameClassic Called... Use another video format!\n";
   //FrameIndex frameIndex = frameIndexFromTimeStamp(newPosition);
   if(isOk(error))
   {
      if(newPosition > duration) error = BadParamsError;
   }
   if(isOk(error))
   {
      if(compareTimeStamps(position, newPosition) && frameFlag)
      {
         error = OkCode;
      }
      else if(newPosition > position || !frameFlag)
      {
         bool keyFrame = false;
         for(error = placeAtNextFrame(true); isOk(error) || !frameFlag;)
         {
            AVPacket *pkt = videoBuffer->readNext();
            if((keyFrame == false) && (((pkt->flags & PKT_FLAG_KEY) && (newPosition - position < 5000)) || (newPosition - position < 3500)))
            {
               keyFrame = true;
               error = decodeFrame();
            }
            else
            {
               if(keyFrame) error = nextFrame();
               else
               {
                 AVPacket *pkt = videoBuffer->extractNext();
                 av_free_packet(pkt);
                 error = placeAtNextFrame(true);
               }
            }
            
            if(compareTimeStamps(position, newPosition)  || newPosition <= position) break;
            //if(newPosition <= position) break;
            /*
            error = nextFrame();
            if(position > newPosition) break;
            */
         }
      }
      else
      {
        cout << "Begin reading video again" << endl;
         error = close();
         if(isOk(error))
         {
            error = _open();
         }
         if(isOk(error))
         {
            error = setFrameByTime(newPosition);
         }
      }

   }

   return error;
}
ReturnCode Decoder::setFrame(FrameIndex frameIndex)
{
    return _setFrame(timeStampFromFrameIndex(frameIndex));
}
ReturnCode Decoder::setFrameByTime(double seconds)
{
   ReturnCode error = OkCode;
   error = _setFrame((TimeStamp)(seconds * FOBS_TIMESTAMP_UNITS_D));
   return error;
}
ReturnCode Decoder::setFrameByTime(TimeStamp milliseconds) 
{
   return _setFrame(milliseconds);
}

ReturnCode Decoder::setPosition(TimeStamp milliseconds)
{
    if(isVideoPresent())
    {
        return setFrameByTime(milliseconds);
    }
}

bool Decoder::isVideoPresent()
{
  return videoStreamIndex >= 0;
}

bool Decoder::isAudioPresent()
{
  return audioStreamIndex >= 0;
}
void Decoder::enableAudio(bool flag)
{
  audioEnabledFlag = flag;
}

omnividea::fobs::uint Decoder::getAudioSampleRate()
{
  uint res = 0;
  if(isAudioPresent())
  {
    res = inputFile->streams[audioStreamIndex]->codec->sample_rate;
  }
  return res;
}
omnividea::fobs::uint Decoder::getAudioBitRate()
{
  uint res = 0;
  if(isAudioPresent())
  {
    res = inputFile->streams[audioStreamIndex]->codec->bit_rate / 1000;
  }
  return res;
}


omnividea::fobs::uint Decoder::getAudioChannelNumber()
{
  uint res = 0;
  if(isAudioPresent())
  {
	  res = inputFile->streams[audioStreamIndex]->codec->channels;
  }
	return res;// > 2?2:res;
}

double Decoder::getAudioTime()
{
  return (positionAudio - firstAudioFramePosition)/FOBS_TIMESTAMP_UNITS_D;
}

double Decoder::getTime()
{
    if(isVideoPresent())
    {
        return getFrameTime();//*FOBS_TIMESTAMP_UNITS_D;
    }
    else
    {
        return getAudioTime();//*FOBS_TIMESTAMP_UNITS_D;
    }
}

AVCodecContext *Decoder::getAudioCodec()
{
  if(isAudioPresent()) return inputFile->streams[audioStreamIndex]->codec;
  return NULL;
}
AVCodecContext *Decoder::getVideoCodec()
{
  if(videoStreamIndex >= 0) return inputFile->streams[videoStreamIndex]->codec;
}
