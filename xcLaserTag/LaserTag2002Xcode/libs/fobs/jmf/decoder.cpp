/******************************************************************************
* FOBS Java JMF Native PlugIn 
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

#include <stdint.h>
extern "C"
{

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <jni.h>
#include <assert.h>


#include "com_omnividea_media_parser_video_Parser.h"
#include "Decoder.h"

#define fobs_log_macro cout

using namespace omnividea::fobs;
using namespace std;

static const jint RGBA = 0;
static const jint YUV420 = 1;

static jlong ptr2jlong(void *ptr)
{
	jlong jl = 0;
	if (sizeof(void *) > sizeof(jlong))
	{	fprintf(stderr, "sizeof(void *) > sizeof(jlong)\n");
		return 0;
		//(* (int *) 0) = 0;	// crash.
	}
	
	memcpy(&jl, &ptr, sizeof(void *));
	return jl;
}

static void *jlong2ptr(jlong jl)
{
	
	void *ptr = 0;
	if (sizeof(void *) > sizeof(jlong))
	{	fprintf(stderr, "sizeof(void *) > sizeof(jlong)\n");
		return 0;
		//(* (int *) 0) = 0;	// crash.
	}
	
	memcpy(&ptr, &jl, sizeof(void *));
	return ptr;
}

bool setPeerVal(JNIEnv *env, jobject obj, Decoder *wrapper)
{
    jclass clazz = env->GetObjectClass(obj);
    if (!clazz) {
        return false;
    }

    jfieldID fidPeer = env->GetFieldID(clazz, "peer", "J");
    if (!fidPeer) {
        return false;
    }
    // set the "peer" variable in the containing java class
    env->SetLongField(obj, fidPeer, ptr2jlong(wrapper));
}

bool isBigEndian()
{
    int tmp = 0x12345678;
    char *buf = (char*)(&tmp);
    //printf("%x %x %x %x\n", (int)buf[0], (int)buf[1], (int)buf[2], (int)buf[3]);
    if(buf[0]==0x78) return (jboolean)0;
    else return (jboolean)1;
}

JNIEXPORT jboolean JNICALL Java_com_omnividea_media_parser_video_Parser_avIsBigEndian
  (JNIEnv *env, jobject obj)
{
    return isBigEndian();
}

JNIEXPORT void JNICALL Java_com_omnividea_media_parser_video_Parser_avInit
  (JNIEnv *env, jobject obj, jstring filename)
{
    Decoder *wrapper;
//    fobs_log_macro << "avInit" << endl;
    jclass clazz = env->GetObjectClass(obj);
    jfieldID fidPeer = env->GetFieldID(clazz, "peer", "J");   
    jlong peerVal = env->GetLongField(obj, fidPeer);
    if (peerVal != 0)
    {
      wrapper = (Decoder *) jlong2ptr(peerVal);
      delete wrapper;
      wrapper = NULL;
    }

    const char *str = env->GetStringUTFChars(filename, 0);
    wrapper = new Decoder(str);//(FFMPEGWrapper *) malloc( sizeof(FFMPEGWrapper) );
    if(!wrapper)
        return;

	// set the "peer" variable in the containing java class
    env->SetLongField(obj, fidPeer, ptr2jlong(wrapper));

    return;
}


JNIEXPORT jboolean JNICALL Java_com_omnividea_media_parser_video_Parser_avOpen
  (JNIEnv *env, jobject obj, jlong peer)
{
    Decoder *wrapper;
//    fobs_log_macro << "avOpen" << endl;
    if (peer == 0)
        return (jboolean) 0;
    wrapper = (Decoder *) jlong2ptr(peer);

    ReturnCode p;
	wrapper->setAudioResampleFlag(true);
    p=wrapper->open();
//    fobs_log_macro << "avOpen returned " << p << " being wrapper=" << wrapper << endl;
    if(isError(p) && p != omnividea::fobs::AlreadyInitializedError)
    {
      cout << "OpenError: " << omnividea::fobs::getErrorMessage(p) << endl;
      return (jboolean) 0;
    }
    //cout << "OpenSuccess" <<endl;
    return (jboolean) 1;
}


/*
JNIEXPORT jboolean JNICALL Java_com_omnividea_media_parser_video_Parser_avProcess
  (JNIEnv *env, jobject obj, jlong peer,
   jobject joutBuffer, jlong outBytes, jlong size)
{
    return Java_com_omnividea_media_parser_video_Parser_avProcess
        (env, obj, peer, joutBuffer, outBytes, size, false, RGBA);
}
*/
JNIEXPORT jboolean JNICALL Java_com_omnividea_media_parser_video_Parser_avProcess
  (JNIEnv *env, jobject obj, jlong peer, jobject joutBuffer, jlong outBytes, jlong size , jboolean useNativeBuffers, jint frameFormat)
{
	int *outBuf = (int *) outBytes;
	int outputDone = 0; // false by default
  jlong outBufferSize = outBytes;
    //fobs_log_macro << "avProcess" << endl;
  Decoder *wrapper;    

	if (peer == 0)
		return (jboolean) 0;

  wrapper = (Decoder *) jlong2ptr(peer);
      
  ReturnCode error = wrapper->nextFrame();
  if(isError(error))
  {
    if(error == VideoEndError)
    {
      cout << "End of video stream\n";
      return (jboolean)0;
    }

    cout << "\tVideoProcess Error: "<< omnividea::fobs::getErrorMessage(error) << endl;
    return Java_com_omnividea_media_parser_video_Parser_avProcess
      (env,  obj,  peer,  joutBuffer,  outBytes,  size, useNativeBuffers, frameFormat);
  }

  if (outBytes == 0)
  {
    if(useNativeBuffers)
    {
        outBuf = (int *) env->GetDirectBufferAddress(joutBuffer);
    }
    else
    {
        //outBuf = (int *) env->GetIntArrayElements((jintArray) joutBuffer, NULL);
        jboolean isCopy;
        outBuf = (int *)env->GetPrimitiveArrayCritical((jintArray) joutBuffer, &isCopy);
        //cout << "*** isCopy = " << (isCopy ? 1 : 0) << endl; // verify that there is no buffer copy
    }
    //outBufferSize = env->GetArrayLength((jbyteArray)joutBuffer);
  }

  if(frameFormat == RGBA)
  {
    byte* src = wrapper->getRGBA((char*)outBuf);
  }
  else if(frameFormat == YUV420)
  {
      AVPicture *pict;
      int pxfmt;
      
      wrapper->getRawFrame(&pict, &pxfmt);

      byte *l = pict->data[0];
      byte *cb = pict->data[2];
      byte *cr = pict->data[1];  
      int wi = wrapper->getWidth();
      int he = wrapper->getHeight();
      
      byte *imgP = (byte*)outBuf;

      byte *lLineBegin = l;
      byte *crLineBegin = cr;
      byte *cbLineBegin = cb;

      for(int h = 0; h < he; h++)
      {
        for(int w = 0; w < wi; w+=2) //PROBLEMS IF WIDTH IS ODD
        {
            register byte ele = *l++;
            register byte cerre = *cr++;
            register byte cebe = *cb++;
            *((uint32_t*)imgP++) = ele<<24|cerre<<16|cebe<<8|0xff;

            //*imgP++ = cerre;
            //*imgP = cebe;
            //imgP+=2;
            ele = *l++;
            *((uint32_t*)imgP++) = ele<<24|cerre<<16|cebe<<8|0xff;
            //*((int*)imgP)++ = 0<<24|0<<16|0xff<<8|0;
            //*imgP++ = ele;
            //*imgP++ = cerre;
            //*imgP = cebe;
            //imgP+=2;
        }
        lLineBegin += pict->linesize[0];
        l = lLineBegin;
        if(h%2 != 0) 
        {
          cbLineBegin += pict->linesize[1];
          crLineBegin += pict->linesize[2];
        }
        cr = crLineBegin;
        cb = cbLineBegin;
      }
  }
  else
  {
    cout << "Output frame format requested is not recognized" << endl;
  }

  if (outBytes == 0)
  {
      if(!useNativeBuffers)env->ReleasePrimitiveArrayCritical((jintArray) joutBuffer, (jint *) outBuf, 0);
      //if(!useNativeBuffers)env->ReleaseIntArrayElements((jintArray) joutBuffer, (jint *) outBuf, 0);
  }

  return (jboolean) 1; 
}

JNIEXPORT jboolean JNICALL Java_com_omnividea_media_parser_video_Parser_avClose
  (JNIEnv *env, jobject obj, jlong peer, jboolean freePeer)
{
	Decoder *wrapper;    
//    fobs_log_macro << "avClose" << endl;
	if (peer == 0)
		return (jboolean) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  
    wrapper->close();
//    fobs_log_macro << "avClose returned " << "being wrapper=" << wrapper << endl;
	if (freePeer) {
		delete wrapper;
        cout << "deleting wrapper" << endl;
		setPeerVal(env, obj, 0);
	}
	return (jboolean) 1; 
}


JNIEXPORT jdouble JNICALL Java_com_omnividea_media_parser_video_Parser_avGetBitRate
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jdouble) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return (jdouble)wrapper->getBitRate();

}


JNIEXPORT jdouble JNICALL Java_com_omnividea_media_parser_video_Parser_avGetDurationSec
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jdouble) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return (jdouble)wrapper->getDurationSeconds();
}



JNIEXPORT jfloat JNICALL Java_com_omnividea_media_parser_video_Parser_avGetFrameRate
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jfloat) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return (jfloat)wrapper->getFrameRate();
}



JNIEXPORT jint JNICALL Java_com_omnividea_media_parser_video_Parser_avGetHeight
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jint) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return (jint)wrapper->getHeight();
}



JNIEXPORT jdouble JNICALL Java_com_omnividea_media_parser_video_Parser_avGetNextFrameTime
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jdouble) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
	jdouble ts = (jdouble)wrapper->getNextFrameTime();
	//cout << "TimeStamp="<<ts<<endl;
  return ts;
}



JNIEXPORT jint JNICALL Java_com_omnividea_media_parser_video_Parser_avGetWidth
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jint) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return (jint)wrapper->getWidth();
}

JNIEXPORT jboolean JNICALL Java_com_omnividea_media_parser_video_Parser_avIsSeekable
  (JNIEnv *env, jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jint) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return (jboolean)true;
}

JNIEXPORT jdouble JNICALL Java_com_omnividea_media_parser_video_Parser_avSetPosition
  (JNIEnv *env, jobject obj, jlong peer, jdouble newTime)
{
	Decoder *wrapper;    
//    fobs_log_macro << "avSetPosition" << endl;
	if (peer == 0)
		return (jint) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  cout << "New position request: " << newTime << endl;
//  fobs_log_macro << "avSetPosition " << " being wrapper=" << wrapper << endl;
  //while(wrapper->testOpen() != OkCode);
  ReturnCode error = wrapper->setFrameByTime(newTime);
  return wrapper->getTime();
}

JNIEXPORT jdouble JNICALL Java_com_omnividea_media_parser_video_Parser_avGetAudioBitRate
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jdouble) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return (jdouble)wrapper->getAudioBitRate();
}
JNIEXPORT jint JNICALL Java_com_omnividea_media_parser_video_Parser_avGetAudioChannelNumber
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jint) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
	cout << "Number of channels: " << wrapper->getAudioChannelNumber() << endl;
  return wrapper->getAudioChannelNumber();
}

JNIEXPORT jint JNICALL Java_com_omnividea_media_parser_video_Parser_avGetAudioSampleSize
  (JNIEnv *env , jobject obj, jlong peer)
{

	Decoder *wrapper;    

	if (peer == 0)
		return (jint) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return wrapper->getAudioSamplesSize();
}
JNIEXPORT jdouble JNICALL Java_com_omnividea_media_parser_video_Parser_avGetAudioSampleTimestamp
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jint) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return wrapper->getAudioTime();
}

JNIEXPORT jboolean JNICALL Java_com_omnividea_media_parser_video_Parser_avIsVideoPresent
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jboolean) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return wrapper->isVideoPresent();
}

JNIEXPORT jboolean JNICALL Java_com_omnividea_media_parser_video_Parser_avIsAudioPresent
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jboolean) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return wrapper->isAudioPresent();
}

JNIEXPORT jdouble JNICALL Java_com_omnividea_media_parser_video_Parser_avGetAudioSampleRate
  (JNIEnv *env , jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jdouble) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return (jdouble)wrapper->getAudioSampleRate();
}


JNIEXPORT jboolean JNICALL Java_com_omnividea_media_parser_video_Parser_avProcessAudio
  (JNIEnv *env, jobject obj, jlong peer,
   jobject joutBuffer, jlong outBytes, jlong size)
{
	unsigned char *outBuf = (unsigned char *) outBytes;
	//int outputDone = 0; // false by default
  jlong outBufferSize = outBytes;

  Decoder *wrapper;    

	if (peer == 0)
		return (jboolean) 0;

  wrapper = (Decoder *) jlong2ptr(peer);
      
  ReturnCode error = wrapper->nextAudioFrame();
  if(isError(error))
  {
    if(error == VideoEndError)
    {
      cout << "End of audio stream\n";
      return (jboolean)0;
    }
    cout << "Audio Error: " << omnividea::fobs::getErrorMessage(error)<< endl;
    return Java_com_omnividea_media_parser_video_Parser_avProcessAudio
      (env, obj,peer, joutBuffer, outBytes, size);
  }

  if (outBytes == 0)
  {
    outBuf = (unsigned char *) env->GetByteArrayElements((jbyteArray) joutBuffer, NULL);
    //outBufferSize = env->GetArrayLength((jbyteArray)joutBuffer);
  }
  
  //printf("Frame Extracted... copy into Java Buffer size=%lf\n", (double)size);
  uint8_t* data = wrapper->getAudioSamples();
  unsigned int i = 0, offset = 0, sizeOut = wrapper->getAudioSamplesSize();
  *outBuf = 0;
  memcpy(outBuf, data, sizeOut);

  if (outBytes == 0)
      env->ReleaseByteArrayElements((jbyteArray) joutBuffer, (jbyte *) outBuf, 0);
  return (jboolean) 1; 
}


/*
 * Class:     disam_media_parser_video_Parser
 * Method:    avGetFirstVideoTime
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_com_omnividea_media_parser_video_Parser_avGetFirstAudioTime
  (JNIEnv *env, jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jdouble) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return wrapper->getFirstAudioSampleTime();
}

/*
 * Class:     disam_media_parser_video_Parser
 * Method:    avGetFirstFrameTime
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_com_omnividea_media_parser_video_Parser_avGetFirstVideoTime
  (JNIEnv *env, jobject obj, jlong peer)
{
	Decoder *wrapper;    

	if (peer == 0)
		return (jdouble) 0;

	wrapper = (Decoder *) jlong2ptr(peer);
  return wrapper->getFirstFrameTime();
}
