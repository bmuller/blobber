/******************************************************************************
* FOBS C++ wrapper code 
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


extern "C"
{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}

#include "Transcoder.h"
#include "PacketBuffer.h"
#include <iostream>

using namespace std;
using namespace omnividea::fobs;

static Decoder iDecoder("");

Transcoder::Transcoder(char *fIn, char *fOut)
{
   completion  = 0.0;
   this->fIn = strdup(fIn);
   this->fOut = strdup(fOut);
   vWidth = -1; //from Input
   vHeight = -1; //from Input
   vBitRate = 0.0; //from Input
   vFrameRate = 0.0; //from Input
   
   aSamplesPerSec = -1; //from Input
   aChannels = -1; //from Input
   aBitRate = 0.0; //from Input
   
   vCodec = NULL; //from Input
   aCodec = NULL; //from Input
   mFormat = NULL; //from Input
   
   customVideoSettings = false;
   customAudioSettings = false;
      
   abortSignal = false;
   
   e = new Encoder(fOut);
   d = new Decoder(fIn);
 
}
Transcoder::~Transcoder()
{
  free(fIn);
  free(fOut);
  if(vCodec) free(vCodec);
  if(aCodec) free(aCodec);
  if(mFormat) free(mFormat);
  
  if(e)e->close();
  if(d)d->close();
  delete e;
  delete d;
}
   
ReturnCode Transcoder::chooseVideoCodec(int width, int height, double bitRate, double frameRate, char *fcc)
{
  vWidth = width;
  vHeight = height;
  vBitRate = bitRate;
  vFrameRate = frameRate;
  if(vCodec) free(vCodec);
  if(fcc) vCodec = strdup(fcc);
  else vCodec = NULL;
  customVideoSettings = true;
  return OkCode;
}
ReturnCode Transcoder::chooseAudioCodec(int samplesPerSec, int channels, double bitRate, char *fcc)
{
  aSamplesPerSec = samplesPerSec;
  aChannels = channels;
  aBitRate = bitRate;
  if(aCodec) free(aCodec);
  if(fcc) aCodec = strdup(fcc);
  else aCodec = NULL;
  customAudioSettings = true;
  return OkCode;
}

ReturnCode Transcoder::chooseFormat(char *fcc)
{
  if(mFormat) free(mFormat);
  if(fcc)mFormat = strdup(fcc);
  else mFormat = NULL;
  e->chooseFormat(fcc);
  return OkCode;
}

ReturnCode Transcoder::transcode()
{
  ReturnCode error = OkCode;
  AVPacket *pkt;
  int vCounter = 0;
  int aCounter = 0;
  d->setAudioResampleFlag(true);
  error = d->open();
  
  if(isOk(error))
  {
      if(customVideoSettings)
      {
        error = e->setVideoParameters(vWidth, vHeight, vBitRate, vFrameRate, vCodec);
      }
      else
      {
        error = e->setVideoParameters(d);
      }
  }
  if(isOk(error))
  {
      if(customAudioSettings)
      {
        error = e->setAudioParameters(aSamplesPerSec, aChannels, aBitRate, aCodec);
      }
      else
      {
        error = e->setAudioParameters(d);
      }
  }
  TimeStamp duration;
  if(isOk(error))
  {
    error = e->open();
    duration = d->getDurationMilliseconds();
  }

  if(isError(error))
  {
    d->close();
    e->close();
    return error;
  }

  while(isOk(error) && abortSignal == false)
  {
    AVFrame avframe; //FIXME/XXX remove this
    AVPacket opkt;
    bool packetFrame;
    av_init_packet(&opkt);

    avcodec_get_frame_defaults(&avframe);   

    packetFrame = false;
    error = d->readNextFrame();
    if(isOk(error))
    {
      while(d->videoBuffer->count() > 0)
      {
        error = d->placeAtNextFrame(true);
        if(customVideoSettings)
        {
            error = d->decodeFrame();
            if(error == NoFrameError)
            {
                error = OkCode;
                continue;
            }
            if(isOk(error))
            {
                byte *rgb = d->getRGB();
                if(rgb == NULL) error = GenericError;
                if(isOk(error))
                {
                    log(Packets, "Adding video frame: %u\n", ++vCounter);
                    e->addFrame((char *)rgb, d->getWidth(), d->getHeight());
                }
            }
        }
        else
        {
            pkt = d->videoBuffer->extractNext();
            e->videoStream->codec->coded_frame = &avframe;
            opkt.stream_index= e->videoStreamIndex;
            e->videoStream->codec->frame_number++;
            packetFrame = true;

            AVRational iTimebase = d->inputFile->streams[d->videoStreamIndex]->time_base;
            AVRational oTimebase = e->videoStream->time_base;

            //opkt.pts = (int64_t)(pkt->dts*iTimebase.num/(double)iTimebase.den * (double)oTimebase.den/oTimebase.num);
            //opkt.dts = (int64_t)(pkt->dts*iTimebase.num/(double)iTimebase.den * (double)oTimebase.den/oTimebase.num);
            if(pkt->pts != AV_NOPTS_VALUE) {
                opkt.pts= av_rescale_q(av_rescale_q(pkt->pts, iTimebase, AV_TIME_BASE_Q) - e->start_time, AV_TIME_BASE_Q,  oTimebase);
                //opkt.pts = av_rescale_q(pkt->pts, iTimebase, oTimebase);
            } else {
                opkt.pts = AV_NOPTS_VALUE;
            }
            {
                int64_t dts;
                if (pkt->dts == AV_NOPTS_VALUE)
                    dts = AV_NOPTS_VALUE;//ist->next_pts;
                else
                    dts= av_rescale_q(pkt->dts, iTimebase, AV_TIME_BASE_Q);
                opkt.dts= av_rescale_q(dts - e->start_time, AV_TIME_BASE_Q,  oTimebase);

                //opkt.dts = av_rescale_q(pkt->dts, iTimebase, oTimebase);
            }

            //cout << "Video Pkt " << pkt->dts << " - Converted into= " << opkt.dts << endl;
        }

      }
//      else
      while(d->audioBuffer->count() > 0)
      {
        error = d->placeAtNextFrame(false);
        if(!isOk(error)) {
            log(Packets, "Error while placing at next audio frame %d\n", error);
            continue;
        }
        if(customAudioSettings)
        {
            error = d->decodeAudioFrame();
            if(isOk(error) && d->getAudioSamplesSize()>0)
            {
                log(Packets, "Adding audio frame: %u\n", ++aCounter);
                if(d->getAudioSamplesSize() <= 0) 
                {
                    log(Packets, "Audio frame was empty!\n");	
                }
                else {
                   e->addAudioFrame(d->getAudioSamples(), d->getAudioSamplesSize(), d->getAudioChannelNumber(), d->getAudioSampleRate());
                }
            }
        }
        else
        {
            //error = d->placeAtNextFrame(false);
            pkt = d->audioBuffer->extractNext();
            e->audioStream->codec->coded_frame = &avframe;
            opkt.stream_index= e->audioStreamIndex;
            e->audioStream->codec->frame_number++;
            packetFrame = true;

            AVRational iTimebase = d->inputFile->streams[d->audioStreamIndex]->time_base;
            AVRational oTimebase = e->audioStream->time_base;
            //opkt.pts = (int64_t)(pkt->dts*iTimebase.num/(double)iTimebase.den * (double)oTimebase.den/oTimebase.num);
            //opkt.dts = (int64_t)(pkt->dts*iTimebase.num/(double)iTimebase.den * (double)oTimebase.den/oTimebase.num);
            if(pkt->pts != AV_NOPTS_VALUE) {
                opkt.pts= av_rescale_q(av_rescale_q(pkt->pts, iTimebase, AV_TIME_BASE_Q) - e->start_time, AV_TIME_BASE_Q,  oTimebase);
                //opkt.pts = av_rescale_q(pkt->pts, iTimebase, oTimebase);
            } else {
                opkt.pts = AV_NOPTS_VALUE;
            }
            {
                int64_t dts;
                if (pkt->dts == AV_NOPTS_VALUE)
                    dts = AV_NOPTS_VALUE;//ist->next_pts;
                else
                    dts= av_rescale_q(pkt->dts, iTimebase, AV_TIME_BASE_Q);
                opkt.dts= av_rescale_q(dts - e->start_time, AV_TIME_BASE_Q,  oTimebase);

                //opkt.dts = av_rescale_q(pkt->dts, iTimebase, oTimebase);
            }
        }
        //cout << "Audio Pkt " << pkt->dts << endl;
      }
    }

    if(packetFrame)
    { 
        avframe.key_frame = pkt->flags & PKT_FLAG_KEY; 
        //opkt.data=  pkt->data;
        //opkt.size=  pkt->size;
        
        //opkt.pts= pkt->pts; //FIXME ist->pts?
        //opkt.dts= pkt->dts;
        
        opkt.flags= pkt->flags;

        
        if(av_parser_change(d->inputFile->streams[pkt->stream_index]->parser, 
                            opkt.stream_index==e->videoStreamIndex?e->videoStream->codec:e->audioStream->codec, 
                            &opkt.data, &opkt.size, pkt->data, pkt->size, pkt->flags & PKT_FLAG_KEY)) 
        {
            opkt.destruct= av_destruct_packet;
        }
        e->addPacket(&opkt);

        av_free_packet(&opkt);
        av_free_packet(pkt);
    }
    
    double p = d->getNextFrameTime()*100000.0/duration;
    if(p > 100.0) p = 100.0;
    completion = p;
    
  }
}

double Transcoder::getCompletion()
{
  return completion;
}

void Transcoder::abort()
{
  abortSignal = true;
}
