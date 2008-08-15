#ifdef __TAGS
#include "libavformat/riff.h"
//*******************************************************************
//The following structures have been extracted from libavformat/mov.c
//*******************************************************************

static const AVCodecTag mov_video_tags[] = {
/*  { CODEC_ID_, MKTAG('c', 'v', 'i', 'd') }, *//* Cinepak */
/*  { CODEC_ID_H263, MKTAG('r', 'a', 'w', ' ') }, *//* Uncompressed RGB */
/*  { CODEC_ID_H263, MKTAG('Y', 'u', 'v', '2') }, *//* Uncompressed YUV422 */
/*    { CODEC_ID_RAWVIDEO, MKTAG('A', 'V', 'U', 'I') }, *//* YUV with alpha-channel (AVID Uncompressed) */
/* Graphics */
/* Animation */
/* Apple video */
/* Kodak Photo CD */
    { CODEC_ID_MJPEG, MKTAG('j', 'p', 'e', 'g') }, /* PhotoJPEG */
    { CODEC_ID_MPEG1VIDEO, MKTAG('m', 'p', 'e', 'g') }, /* MPEG */
    { CODEC_ID_MJPEG, MKTAG('m', 'j', 'p', 'a') }, /* Motion-JPEG (format A) */
    { CODEC_ID_MJPEG, MKTAG('m', 'j', 'p', 'b') }, /* Motion-JPEG (format B) */
    { CODEC_ID_MJPEG, MKTAG('A', 'V', 'D', 'J') }, /* MJPEG with alpha-channel (AVID JFIF meridien compressed) */
/*    { CODEC_ID_MJPEG, MKTAG('A', 'V', 'R', 'n') }, *//* MJPEG with alpha-channel (AVID ABVB/Truevision NuVista) */
/*    { CODEC_ID_GIF, MKTAG('g', 'i', 'f', ' ') }, *//* embedded gif files as frames (usually one "click to play movie" frame) */
/* Sorenson video */
    { CODEC_ID_SVQ1, MKTAG('S', 'V', 'Q', '1') }, /* Sorenson Video v1 */
    { CODEC_ID_SVQ1, MKTAG('s', 'v', 'q', '1') }, /* Sorenson Video v1 */
    { CODEC_ID_SVQ1, MKTAG('s', 'v', 'q', 'i') }, /* Sorenson Video v1 (from QT specs)*/
    { CODEC_ID_SVQ3, MKTAG('S', 'V', 'Q', '3') }, /* Sorenson Video v3 */
    { CODEC_ID_MPEG4, MKTAG('m', 'p', '4', 'v') },
    { CODEC_ID_MPEG4, MKTAG('D', 'I', 'V', 'X') }, /* OpenDiVX *//* sample files at http://heroinewarrior.com/xmovie.php3 use this tag */
    { CODEC_ID_MPEG4, MKTAG('X', 'V', 'I', 'D') },
    { CODEC_ID_MPEG4, MKTAG('3', 'I', 'V', '2') }, /* experimental: 3IVX files before ivx D4 4.5.1 */
/*    { CODEC_ID_, MKTAG('I', 'V', '5', '0') }, *//* Indeo 5.0 */
    { CODEC_ID_H263, MKTAG('h', '2', '6', '3') }, /* H263 */
    { CODEC_ID_H263, MKTAG('s', '2', '6', '3') }, /* H263 ?? works */
    { CODEC_ID_DVVIDEO, MKTAG('d', 'v', 'c', ' ') }, /* DV NTSC */
    { CODEC_ID_DVVIDEO, MKTAG('d', 'v', 'c', 'p') }, /* DV PAL */
/*    { CODEC_ID_DVVIDEO, MKTAG('A', 'V', 'd', 'v') }, *//* AVID dv */
    { CODEC_ID_VP3, MKTAG('V', 'P', '3', '1') }, /* On2 VP3 */
    { CODEC_ID_RPZA, MKTAG('r', 'p', 'z', 'a') }, /* Apple Video (RPZA) */
    { CODEC_ID_CINEPAK, MKTAG('c', 'v', 'i', 'd') }, /* Cinepak */
    { CODEC_ID_8BPS, MKTAG('8', 'B', 'P', 'S') }, /* Planar RGB (8BPS) */
    { CODEC_ID_SMC, MKTAG('s', 'm', 'c', ' ') }, /* Apple Graphics (SMC) */
    { CODEC_ID_QTRLE, MKTAG('r', 'l', 'e', ' ') }, /* Apple Animation (RLE) */
    { CODEC_ID_QDRAW, MKTAG('q', 'd', 'r', 'w') }, /* QuickDraw */
    { CODEC_ID_H264, MKTAG('a', 'v', 'c', '1') }, /* AVC-1/H.264 */
    { CODEC_ID_MPEG2VIDEO, MKTAG('h', 'd', 'v', '2') }, /* MPEG2 produced by Sony HD camera */
    { CODEC_ID_NONE, 0 },
};

static const AVCodecTag mov_audio_tags[] = {
/*    { CODEC_ID_PCM_S16BE, MKTAG('N', 'O', 'N', 'E') }, *//* uncompressed */
    { CODEC_ID_PCM_S16BE, MKTAG('t', 'w', 'o', 's') }, /* 16 bits */
    /* { CODEC_ID_PCM_S8, MKTAG('t', 'w', 'o', 's') },*/ /* 8 bits */
    { CODEC_ID_PCM_U8, MKTAG('r', 'a', 'w', ' ') }, /* 8 bits unsigned */
    { CODEC_ID_PCM_S16LE, MKTAG('s', 'o', 'w', 't') }, /*  */
    { CODEC_ID_PCM_MULAW, MKTAG('u', 'l', 'a', 'w') }, /*  */
    { CODEC_ID_PCM_ALAW, MKTAG('a', 'l', 'a', 'w') }, /*  */
    { CODEC_ID_ADPCM_IMA_QT, MKTAG('i', 'm', 'a', '4') }, /* IMA-4 ADPCM */
    { CODEC_ID_MACE3, MKTAG('M', 'A', 'C', '3') }, /* Macintosh Audio Compression and Expansion 3:1 */
    { CODEC_ID_MACE6, MKTAG('M', 'A', 'C', '6') }, /* Macintosh Audio Compression and Expansion 6:1 */

    { CODEC_ID_MP2, MKTAG('.', 'm', 'p', '3') }, /* MPEG layer 3 */ /* sample files at http://www.3ivx.com/showcase.html use this tag */
    { CODEC_ID_MP2, 0x6D730055 }, /* MPEG layer 3 */
    { CODEC_ID_MP2, 0x5500736D }, /* MPEG layer 3 *//* XXX: check endianness */
/*    { CODEC_ID_OGG_VORBIS, MKTAG('O', 'g', 'g', 'S') }, *//* sample files at http://heroinewarrior.com/xmovie.php3 use this tag */
/* MP4 tags */
    { CODEC_ID_AAC, MKTAG('m', 'p', '4', 'a') }, /* MPEG-4 AAC */
    /* The standard for mpeg4 audio is still not normalised AFAIK anyway */
    { CODEC_ID_AMR_NB, MKTAG('s', 'a', 'm', 'r') }, /* AMR-NB 3gp */
    { CODEC_ID_AMR_WB, MKTAG('s', 'a', 'w', 'b') }, /* AMR-WB 3gp */
    { CODEC_ID_AC3, MKTAG('m', 's', 0x20, 0x00) }, /* Dolby AC-3 */
    { CODEC_ID_ALAC,MKTAG('a', 'l', 'a', 'c') }, /* Apple Lossless */
    { CODEC_ID_NONE, 0 },
};



//*******************************************************************
//The following structures have been extracted from libavformat/nsvdec.c
//*******************************************************************
static const AVCodecTag nsv_codec_video_tags[] = {
    { CODEC_ID_VP3, MKTAG('V', 'P', '3', ' ') },
    { CODEC_ID_VP3, MKTAG('V', 'P', '3', '0') },
    { CODEC_ID_VP3, MKTAG('V', 'P', '3', '1') },
/*
    { CODEC_ID_VP4, MKTAG('V', 'P', '4', ' ') },
    { CODEC_ID_VP4, MKTAG('V', 'P', '4', '0') },
    { CODEC_ID_VP5, MKTAG('V', 'P', '5', ' ') },
    { CODEC_ID_VP5, MKTAG('V', 'P', '5', '0') },
    { CODEC_ID_VP6, MKTAG('V', 'P', '6', ' ') },
    { CODEC_ID_VP6, MKTAG('V', 'P', '6', '0') },
    { CODEC_ID_VP6, MKTAG('V', 'P', '6', '1') },
    { CODEC_ID_VP6, MKTAG('V', 'P', '6', '2') },
*/
    { CODEC_ID_XVID, MKTAG('X', 'V', 'I', 'D') }, /* cf sample xvid decoder from nsv_codec_sdk.zip */
    { CODEC_ID_RAWVIDEO, MKTAG('R', 'G', 'B', '3') },
    { 0, 0 },
};

static const AVCodecTag nsv_codec_audio_tags[] = {
    { CODEC_ID_MP3, MKTAG('M', 'P', '3', ' ') },
    { CODEC_ID_AAC, MKTAG('A', 'A', 'C', ' ') },
    { CODEC_ID_AAC, MKTAG('A', 'A', 'C', 'P') }, /* _CUTTED__MUXED_2 Heads - Out Of The City.nsv */
    { CODEC_ID_PCM_U16LE, MKTAG('P', 'C', 'M', ' ') },
    { 0, 0 },
};


#endif