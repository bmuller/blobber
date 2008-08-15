# Automatically generated by configure - do not modify!
prefix=/home/zenko/Internets/xcLaserTag/LaserTag2002Xcode/libs/fobs//external
LIBDIR=$(DESTDIR)${prefix}/lib
SHLIBDIR=$(DESTDIR)${prefix}/lib
INCDIR=$(DESTDIR)${prefix}/include
BINDIR=$(DESTDIR)${prefix}/bin
MANDIR=$(DESTDIR)${prefix}/share/man
CC=gcc
AR=ar
RANLIB=ranlib
LN_S=ln -sf
STRIP=strip
OPTFLAGS= -fomit-frame-pointer -g -Wdeclaration-after-statement -Wall -Wno-switch -Wdisabled-optimization -Wpointer-arith -Wredundant-decls -Wno-pointer-sign -Wcast-qual -Wwrite-strings -O3 -fno-math-errno
VHOOKCFLAGS=-fPIC
LDFLAGS=  -rdynamic -export-dynamic -Wl,--warn-common -Wl,--as-needed -Wl,-rpath-link,$(BUILD_ROOT)/libpostproc -Wl,-rpath-link,$(BUILD_ROOT)/libswscale -Wl,-rpath-link,$(BUILD_ROOT)/libavfilter -Wl,-rpath-link,$(BUILD_ROOT)/libavdevice -Wl,-rpath-link,$(BUILD_ROOT)/libavformat -Wl,-rpath-link,$(BUILD_ROOT)/libavcodec -Wl,-rpath-link,$(BUILD_ROOT)/libavutil -Wl,-Bsymbolic
FFSERVERLDFLAGS=-Wl,-E
SHFLAGS=-shared -Wl,-soname,$$(@F)
VHOOKSHFLAGS=$(SHFLAGS)
VHOOKLIBS=
LIBOBJFLAGS=
BUILD_STATIC=yes
BUILDSUF=
FULLNAME=$(NAME)$(BUILDSUF)
LIBPREF=lib
LIBSUF=.a
LIBNAME=$(LIBPREF)$(FULLNAME)$(LIBSUF)
SLIBPREF=lib
SLIBSUF=.so
EXESUF=
DEPEND_CMD=$(CC) $(CFLAGS) -MM -MG $< | sed -e "/^\#.*/d" -e "1s,^,$(@D)/,"
libswscale_VERSION=0.5.1
libswscale_VERSION_MAJOR=0
libpostproc_VERSION=51.1.0
libpostproc_VERSION_MAJOR=51
libavcodec_VERSION=51.57.2
libavcodec_VERSION_MAJOR=51
libavdevice_VERSION=52.0.0
libavdevice_VERSION_MAJOR=52
libavformat_VERSION=52.16.0
libavformat_VERSION_MAJOR=52
libavutil_VERSION=49.7.0
libavutil_VERSION_MAJOR=49
libavfilter_VERSION=0.0.0
libavfilter_VERSION_MAJOR=0
LIB_INSTALL_EXTRA_CMD=$(RANLIB) "$(LIBDIR)/$(LIBNAME)"
EXTRALIBS=  -lz -lm  -ldl -ldl
ARCH_X86=yes
ARCH_X86_32=yes
HAVE_MMX=yes
HAVE_MMX2=yes
HAVE_SSSE3=yes
HAVE_ARPA_INET_H=yes
HAVE_BSWAP=yes
HAVE_BYTESWAP_H=yes
HAVE_DLFCN_H=yes
HAVE_DLOPEN=yes
HAVE_EBP_AVAILABLE=yes
HAVE_EBX_AVAILABLE=yes
HAVE_FAST_UNALIGNED=yes
HAVE_FORK=yes
HAVE_FREETYPE2=yes
HAVE_GETRUSAGE=yes
HAVE_INET_ATON=yes
HAVE_INLINE_ASM=yes
HAVE_LLRINT=yes
HAVE_LRINT=yes
HAVE_LRINTF=yes
HAVE_MALLOC_H=yes
HAVE_MEMALIGN=yes
HAVE_MKSTEMP=yes
HAVE_ROUND=yes
HAVE_ROUNDF=yes
HAVE_SOCKLEN_T=yes
HAVE_POLL_H=yes
HAVE_SYS_MMAN_H=yes
HAVE_SYS_RESOURCE_H=yes
HAVE_SYS_SELECT_H=yes
HAVE_SYS_SOUNDCARD_H=yes
HAVE_TERMIOS_H=yes
CONFIG_BSFS=yes
CONFIG_DECODERS=yes
CONFIG_DEMUXERS=yes
CONFIG_ENCODERS=yes
CONFIG_FILTERS=yes
CONFIG_MUXERS=yes
CONFIG_PROTOCOLS=yes
CONFIG_FFMPEG=yes
CONFIG_FFSERVER=yes
CONFIG_IPV6=yes
CONFIG_MPEGAUDIO_HP=yes
CONFIG_NETWORK=yes
CONFIG_VHOOK=yes
CONFIG_ZLIB=yes
CONFIG_AASC_DECODER=yes
CONFIG_AMV_DECODER=yes
CONFIG_ASV1_DECODER=yes
CONFIG_ASV2_DECODER=yes
CONFIG_AVS_DECODER=yes
CONFIG_BETHSOFTVID_DECODER=yes
CONFIG_BFI_DECODER=yes
CONFIG_BMP_DECODER=yes
CONFIG_C93_DECODER=yes
CONFIG_CAVS_DECODER=yes
CONFIG_CINEPAK_DECODER=yes
CONFIG_CLJR_DECODER=yes
CONFIG_CSCD_DECODER=yes
CONFIG_CYUV_DECODER=yes
CONFIG_DNXHD_DECODER=yes
CONFIG_DSICINVIDEO_DECODER=yes
CONFIG_DVVIDEO_DECODER=yes
CONFIG_DXA_DECODER=yes
CONFIG_EIGHTBPS_DECODER=yes
CONFIG_EIGHTSVX_EXP_DECODER=yes
CONFIG_EIGHTSVX_FIB_DECODER=yes
CONFIG_ESCAPE124_DECODER=yes
CONFIG_FFV1_DECODER=yes
CONFIG_FFVHUFF_DECODER=yes
CONFIG_FLASHSV_DECODER=yes
CONFIG_FLIC_DECODER=yes
CONFIG_FLV_DECODER=yes
CONFIG_FOURXM_DECODER=yes
CONFIG_FRAPS_DECODER=yes
CONFIG_GIF_DECODER=yes
CONFIG_H261_DECODER=yes
CONFIG_H263_DECODER=yes
CONFIG_H263I_DECODER=yes
CONFIG_H264_DECODER=yes
CONFIG_HUFFYUV_DECODER=yes
CONFIG_IDCIN_DECODER=yes
CONFIG_INDEO2_DECODER=yes
CONFIG_INDEO3_DECODER=yes
CONFIG_INTERPLAY_VIDEO_DECODER=yes
CONFIG_JPEGLS_DECODER=yes
CONFIG_KMVC_DECODER=yes
CONFIG_LOCO_DECODER=yes
CONFIG_MDEC_DECODER=yes
CONFIG_MIMIC_DECODER=yes
CONFIG_MJPEG_DECODER=yes
CONFIG_MJPEGB_DECODER=yes
CONFIG_MMVIDEO_DECODER=yes
CONFIG_MPEG1VIDEO_DECODER=yes
CONFIG_MPEG2VIDEO_DECODER=yes
CONFIG_MPEG4_DECODER=yes
CONFIG_MPEGVIDEO_DECODER=yes
CONFIG_MSMPEG4V1_DECODER=yes
CONFIG_MSMPEG4V2_DECODER=yes
CONFIG_MSMPEG4V3_DECODER=yes
CONFIG_MSRLE_DECODER=yes
CONFIG_MSVIDEO1_DECODER=yes
CONFIG_MSZH_DECODER=yes
CONFIG_NUV_DECODER=yes
CONFIG_PCX_DECODER=yes
CONFIG_PNG_DECODER=yes
CONFIG_PTX_DECODER=yes
CONFIG_QDRAW_DECODER=yes
CONFIG_QPEG_DECODER=yes
CONFIG_QTRLE_DECODER=yes
CONFIG_RAWVIDEO_DECODER=yes
CONFIG_RL2_DECODER=yes
CONFIG_ROQ_DECODER=yes
CONFIG_RPZA_DECODER=yes
CONFIG_RV10_DECODER=yes
CONFIG_RV20_DECODER=yes
CONFIG_SGI_DECODER=yes
CONFIG_SMACKER_DECODER=yes
CONFIG_SMC_DECODER=yes
CONFIG_SNOW_DECODER=yes
CONFIG_SP5X_DECODER=yes
CONFIG_SUNRAST_DECODER=yes
CONFIG_SVQ1_DECODER=yes
CONFIG_SVQ3_DECODER=yes
CONFIG_TARGA_DECODER=yes
CONFIG_THEORA_DECODER=yes
CONFIG_THP_DECODER=yes
CONFIG_TIERTEXSEQVIDEO_DECODER=yes
CONFIG_TIFF_DECODER=yes
CONFIG_TRUEMOTION1_DECODER=yes
CONFIG_TRUEMOTION2_DECODER=yes
CONFIG_TSCC_DECODER=yes
CONFIG_TXD_DECODER=yes
CONFIG_ULTI_DECODER=yes
CONFIG_VB_DECODER=yes
CONFIG_VC1_DECODER=yes
CONFIG_VCR1_DECODER=yes
CONFIG_VMDVIDEO_DECODER=yes
CONFIG_VMNC_DECODER=yes
CONFIG_VP3_DECODER=yes
CONFIG_VP5_DECODER=yes
CONFIG_VP6_DECODER=yes
CONFIG_VP6A_DECODER=yes
CONFIG_VP6F_DECODER=yes
CONFIG_VQA_DECODER=yes
CONFIG_WMV1_DECODER=yes
CONFIG_WMV2_DECODER=yes
CONFIG_WMV3_DECODER=yes
CONFIG_WNV1_DECODER=yes
CONFIG_XAN_WC3_DECODER=yes
CONFIG_XL_DECODER=yes
CONFIG_XSUB_DECODER=yes
CONFIG_ZLIB_DECODER=yes
CONFIG_ZMBV_DECODER=yes
CONFIG_ALAC_DECODER=yes
CONFIG_APE_DECODER=yes
CONFIG_ATRAC3_DECODER=yes
CONFIG_COOK_DECODER=yes
CONFIG_DCA_DECODER=yes
CONFIG_DSICINAUDIO_DECODER=yes
CONFIG_FLAC_DECODER=yes
CONFIG_IMC_DECODER=yes
CONFIG_MACE3_DECODER=yes
CONFIG_MACE6_DECODER=yes
CONFIG_MP2_DECODER=yes
CONFIG_MP3_DECODER=yes
CONFIG_MP3ADU_DECODER=yes
CONFIG_MP3ON4_DECODER=yes
CONFIG_MPC7_DECODER=yes
CONFIG_MPC8_DECODER=yes
CONFIG_NELLYMOSER_DECODER=yes
CONFIG_QDM2_DECODER=yes
CONFIG_RA_144_DECODER=yes
CONFIG_RA_288_DECODER=yes
CONFIG_SHORTEN_DECODER=yes
CONFIG_SMACKAUD_DECODER=yes
CONFIG_SONIC_DECODER=yes
CONFIG_TRUESPEECH_DECODER=yes
CONFIG_TTA_DECODER=yes
CONFIG_VMDAUDIO_DECODER=yes
CONFIG_VORBIS_DECODER=yes
CONFIG_WAVPACK_DECODER=yes
CONFIG_WMAV1_DECODER=yes
CONFIG_WMAV2_DECODER=yes
CONFIG_WS_SND1_DECODER=yes
CONFIG_PCM_ALAW_DECODER=yes
CONFIG_PCM_DVD_DECODER=yes
CONFIG_PCM_MULAW_DECODER=yes
CONFIG_PCM_S8_DECODER=yes
CONFIG_PCM_S16BE_DECODER=yes
CONFIG_PCM_S16LE_DECODER=yes
CONFIG_PCM_S16LE_PLANAR_DECODER=yes
CONFIG_PCM_S24BE_DECODER=yes
CONFIG_PCM_S24DAUD_DECODER=yes
CONFIG_PCM_S24LE_DECODER=yes
CONFIG_PCM_S32BE_DECODER=yes
CONFIG_PCM_S32LE_DECODER=yes
CONFIG_PCM_U8_DECODER=yes
CONFIG_PCM_U16BE_DECODER=yes
CONFIG_PCM_U16LE_DECODER=yes
CONFIG_PCM_U24BE_DECODER=yes
CONFIG_PCM_U24LE_DECODER=yes
CONFIG_PCM_U32BE_DECODER=yes
CONFIG_PCM_U32LE_DECODER=yes
CONFIG_PCM_ZORK_DECODER=yes
CONFIG_INTERPLAY_DPCM_DECODER=yes
CONFIG_ROQ_DPCM_DECODER=yes
CONFIG_SOL_DPCM_DECODER=yes
CONFIG_XAN_DPCM_DECODER=yes
CONFIG_ADPCM_4XM_DECODER=yes
CONFIG_ADPCM_ADX_DECODER=yes
CONFIG_ADPCM_CT_DECODER=yes
CONFIG_ADPCM_EA_DECODER=yes
CONFIG_ADPCM_EA_MAXIS_XA_DECODER=yes
CONFIG_ADPCM_EA_R1_DECODER=yes
CONFIG_ADPCM_EA_R2_DECODER=yes
CONFIG_ADPCM_EA_R3_DECODER=yes
CONFIG_ADPCM_EA_XAS_DECODER=yes
CONFIG_ADPCM_G726_DECODER=yes
CONFIG_ADPCM_IMA_AMV_DECODER=yes
CONFIG_ADPCM_IMA_DK3_DECODER=yes
CONFIG_ADPCM_IMA_DK4_DECODER=yes
CONFIG_ADPCM_IMA_EA_EACS_DECODER=yes
CONFIG_ADPCM_IMA_EA_SEAD_DECODER=yes
CONFIG_ADPCM_IMA_QT_DECODER=yes
CONFIG_ADPCM_IMA_SMJPEG_DECODER=yes
CONFIG_ADPCM_IMA_WAV_DECODER=yes
CONFIG_ADPCM_IMA_WS_DECODER=yes
CONFIG_ADPCM_MS_DECODER=yes
CONFIG_ADPCM_SBPRO_2_DECODER=yes
CONFIG_ADPCM_SBPRO_3_DECODER=yes
CONFIG_ADPCM_SBPRO_4_DECODER=yes
CONFIG_ADPCM_SWF_DECODER=yes
CONFIG_ADPCM_THP_DECODER=yes
CONFIG_ADPCM_XA_DECODER=yes
CONFIG_ADPCM_YAMAHA_DECODER=yes
CONFIG_DVBSUB_DECODER=yes
CONFIG_DVDSUB_DECODER=yes
CONFIG_ASV1_ENCODER=yes
CONFIG_ASV2_ENCODER=yes
CONFIG_BMP_ENCODER=yes
CONFIG_DNXHD_ENCODER=yes
CONFIG_DVVIDEO_ENCODER=yes
CONFIG_FFV1_ENCODER=yes
CONFIG_FFVHUFF_ENCODER=yes
CONFIG_FLASHSV_ENCODER=yes
CONFIG_FLV_ENCODER=yes
CONFIG_GIF_ENCODER=yes
CONFIG_H261_ENCODER=yes
CONFIG_H263_ENCODER=yes
CONFIG_H263P_ENCODER=yes
CONFIG_HUFFYUV_ENCODER=yes
CONFIG_JPEGLS_ENCODER=yes
CONFIG_LJPEG_ENCODER=yes
CONFIG_MJPEG_ENCODER=yes
CONFIG_MPEG1VIDEO_ENCODER=yes
CONFIG_MPEG2VIDEO_ENCODER=yes
CONFIG_MPEG4_ENCODER=yes
CONFIG_MSMPEG4V1_ENCODER=yes
CONFIG_MSMPEG4V2_ENCODER=yes
CONFIG_MSMPEG4V3_ENCODER=yes
CONFIG_PAM_ENCODER=yes
CONFIG_PBM_ENCODER=yes
CONFIG_PGM_ENCODER=yes
CONFIG_PGMYUV_ENCODER=yes
CONFIG_PNG_ENCODER=yes
CONFIG_PPM_ENCODER=yes
CONFIG_QTRLE_ENCODER=yes
CONFIG_RAWVIDEO_ENCODER=yes
CONFIG_ROQ_ENCODER=yes
CONFIG_RV10_ENCODER=yes
CONFIG_RV20_ENCODER=yes
CONFIG_SGI_ENCODER=yes
CONFIG_SNOW_ENCODER=yes
CONFIG_SVQ1_ENCODER=yes
CONFIG_TARGA_ENCODER=yes
CONFIG_TIFF_ENCODER=yes
CONFIG_WMV1_ENCODER=yes
CONFIG_WMV2_ENCODER=yes
CONFIG_ZLIB_ENCODER=yes
CONFIG_ZMBV_ENCODER=yes
CONFIG_AC3_ENCODER=yes
CONFIG_FLAC_ENCODER=yes
CONFIG_MP2_ENCODER=yes
CONFIG_SONIC_ENCODER=yes
CONFIG_SONIC_LS_ENCODER=yes
CONFIG_VORBIS_ENCODER=yes
CONFIG_WMAV1_ENCODER=yes
CONFIG_WMAV2_ENCODER=yes
CONFIG_PCM_ALAW_ENCODER=yes
CONFIG_PCM_MULAW_ENCODER=yes
CONFIG_PCM_S8_ENCODER=yes
CONFIG_PCM_S16BE_ENCODER=yes
CONFIG_PCM_S16LE_ENCODER=yes
CONFIG_PCM_S24BE_ENCODER=yes
CONFIG_PCM_S24DAUD_ENCODER=yes
CONFIG_PCM_S24LE_ENCODER=yes
CONFIG_PCM_S32BE_ENCODER=yes
CONFIG_PCM_S32LE_ENCODER=yes
CONFIG_PCM_U8_ENCODER=yes
CONFIG_PCM_U16BE_ENCODER=yes
CONFIG_PCM_U16LE_ENCODER=yes
CONFIG_PCM_U24BE_ENCODER=yes
CONFIG_PCM_U24LE_ENCODER=yes
CONFIG_PCM_U32BE_ENCODER=yes
CONFIG_PCM_U32LE_ENCODER=yes
CONFIG_PCM_ZORK_ENCODER=yes
CONFIG_ROQ_DPCM_ENCODER=yes
CONFIG_ADPCM_ADX_ENCODER=yes
CONFIG_ADPCM_G726_ENCODER=yes
CONFIG_ADPCM_IMA_QT_ENCODER=yes
CONFIG_ADPCM_IMA_WAV_ENCODER=yes
CONFIG_ADPCM_MS_ENCODER=yes
CONFIG_ADPCM_SWF_ENCODER=yes
CONFIG_ADPCM_YAMAHA_ENCODER=yes
CONFIG_DVBSUB_ENCODER=yes
CONFIG_DVDSUB_ENCODER=yes
CONFIG_AAC_PARSER=yes
CONFIG_AC3_PARSER=yes
CONFIG_CAVSVIDEO_PARSER=yes
CONFIG_DCA_PARSER=yes
CONFIG_DIRAC_PARSER=yes
CONFIG_DVBSUB_PARSER=yes
CONFIG_DVDSUB_PARSER=yes
CONFIG_H261_PARSER=yes
CONFIG_H263_PARSER=yes
CONFIG_H264_PARSER=yes
CONFIG_MJPEG_PARSER=yes
CONFIG_MLP_PARSER=yes
CONFIG_MPEG4VIDEO_PARSER=yes
CONFIG_MPEGAUDIO_PARSER=yes
CONFIG_MPEGVIDEO_PARSER=yes
CONFIG_PNM_PARSER=yes
CONFIG_VC1_PARSER=yes
CONFIG_DUMP_EXTRADATA_BSF=yes
CONFIG_H264_MP4TOANNEXB_BSF=yes
CONFIG_IMX_DUMP_HEADER_BSF=yes
CONFIG_MJPEGA_DUMP_HEADER_BSF=yes
CONFIG_MP3_HEADER_COMPRESS_BSF=yes
CONFIG_MP3_HEADER_DECOMPRESS_BSF=yes
CONFIG_MOV2TEXTSUB_BSF=yes
CONFIG_NOISE_BSF=yes
CONFIG_REMOVE_EXTRADATA_BSF=yes
CONFIG_TEXT2MOVSUB_BSF=yes
CONFIG_AAC_DEMUXER=yes
CONFIG_AC3_DEMUXER=yes
CONFIG_AIFF_DEMUXER=yes
CONFIG_AMR_DEMUXER=yes
CONFIG_APC_DEMUXER=yes
CONFIG_APE_DEMUXER=yes
CONFIG_ASF_DEMUXER=yes
CONFIG_AU_DEMUXER=yes
CONFIG_AVI_DEMUXER=yes
CONFIG_AVS_DEMUXER=yes
CONFIG_BETHSOFTVID_DEMUXER=yes
CONFIG_BFI_DEMUXER=yes
CONFIG_C93_DEMUXER=yes
CONFIG_DAUD_DEMUXER=yes
CONFIG_DIRAC_DEMUXER=yes
CONFIG_DSICIN_DEMUXER=yes
CONFIG_DTS_DEMUXER=yes
CONFIG_DV_DEMUXER=yes
CONFIG_DXA_DEMUXER=yes
CONFIG_EA_DEMUXER=yes
CONFIG_EA_CDATA_DEMUXER=yes
CONFIG_FFM_DEMUXER=yes
CONFIG_FLAC_DEMUXER=yes
CONFIG_FLIC_DEMUXER=yes
CONFIG_FLV_DEMUXER=yes
CONFIG_FOURXM_DEMUXER=yes
CONFIG_GIF_DEMUXER=yes
CONFIG_GSM_DEMUXER=yes
CONFIG_GXF_DEMUXER=yes
CONFIG_H261_DEMUXER=yes
CONFIG_H263_DEMUXER=yes
CONFIG_H264_DEMUXER=yes
CONFIG_IDCIN_DEMUXER=yes
CONFIG_IFF_DEMUXER=yes
CONFIG_IMAGE2_DEMUXER=yes
CONFIG_IMAGE2PIPE_DEMUXER=yes
CONFIG_INGENIENT_DEMUXER=yes
CONFIG_IPMOVIE_DEMUXER=yes
CONFIG_LMLM4_DEMUXER=yes
CONFIG_M4V_DEMUXER=yes
CONFIG_MATROSKA_DEMUXER=yes
CONFIG_MJPEG_DEMUXER=yes
CONFIG_MLP_DEMUXER=yes
CONFIG_MM_DEMUXER=yes
CONFIG_MMF_DEMUXER=yes
CONFIG_MOV_DEMUXER=yes
CONFIG_MP3_DEMUXER=yes
CONFIG_MPC_DEMUXER=yes
CONFIG_MPC8_DEMUXER=yes
CONFIG_MPEGPS_DEMUXER=yes
CONFIG_MPEGTS_DEMUXER=yes
CONFIG_MPEGTSRAW_DEMUXER=yes
CONFIG_MPEGVIDEO_DEMUXER=yes
CONFIG_MSNWC_TCP_DEMUXER=yes
CONFIG_MTV_DEMUXER=yes
CONFIG_MXF_DEMUXER=yes
CONFIG_NSV_DEMUXER=yes
CONFIG_NUT_DEMUXER=yes
CONFIG_NUV_DEMUXER=yes
CONFIG_OGG_DEMUXER=yes
CONFIG_OMA_DEMUXER=yes
CONFIG_PCM_ALAW_DEMUXER=yes
CONFIG_PCM_MULAW_DEMUXER=yes
CONFIG_PCM_S16BE_DEMUXER=yes
CONFIG_PCM_S16LE_DEMUXER=yes
CONFIG_PCM_S8_DEMUXER=yes
CONFIG_PCM_U16BE_DEMUXER=yes
CONFIG_PCM_U16LE_DEMUXER=yes
CONFIG_PCM_U8_DEMUXER=yes
CONFIG_PVA_DEMUXER=yes
CONFIG_RAWVIDEO_DEMUXER=yes
CONFIG_REDIR_DEMUXER=yes
CONFIG_RL2_DEMUXER=yes
CONFIG_RM_DEMUXER=yes
CONFIG_ROQ_DEMUXER=yes
CONFIG_RPL_DEMUXER=yes
CONFIG_RTSP_DEMUXER=yes
CONFIG_SDP_DEMUXER=yes
CONFIG_SEGAFILM_DEMUXER=yes
CONFIG_SHORTEN_DEMUXER=yes
CONFIG_SIFF_DEMUXER=yes
CONFIG_SMACKER_DEMUXER=yes
CONFIG_SOL_DEMUXER=yes
CONFIG_STR_DEMUXER=yes
CONFIG_SWF_DEMUXER=yes
CONFIG_THP_DEMUXER=yes
CONFIG_TIERTEXSEQ_DEMUXER=yes
CONFIG_TTA_DEMUXER=yes
CONFIG_TXD_DEMUXER=yes
CONFIG_VC1_DEMUXER=yes
CONFIG_VC1T_DEMUXER=yes
CONFIG_VMD_DEMUXER=yes
CONFIG_VOC_DEMUXER=yes
CONFIG_WAV_DEMUXER=yes
CONFIG_WC3_DEMUXER=yes
CONFIG_WSAUD_DEMUXER=yes
CONFIG_WSVQA_DEMUXER=yes
CONFIG_WV_DEMUXER=yes
CONFIG_XA_DEMUXER=yes
CONFIG_YUV4MPEGPIPE_DEMUXER=yes
CONFIG_AC3_MUXER=yes
CONFIG_ADTS_MUXER=yes
CONFIG_AIFF_MUXER=yes
CONFIG_AMR_MUXER=yes
CONFIG_ASF_MUXER=yes
CONFIG_ASF_STREAM_MUXER=yes
CONFIG_AU_MUXER=yes
CONFIG_AVI_MUXER=yes
CONFIG_AVM2_MUXER=yes
CONFIG_CRC_MUXER=yes
CONFIG_DIRAC_MUXER=yes
CONFIG_DTS_MUXER=yes
CONFIG_DV_MUXER=yes
CONFIG_FFM_MUXER=yes
CONFIG_FLAC_MUXER=yes
CONFIG_FLV_MUXER=yes
CONFIG_FRAMECRC_MUXER=yes
CONFIG_GIF_MUXER=yes
CONFIG_GXF_MUXER=yes
CONFIG_H261_MUXER=yes
CONFIG_H263_MUXER=yes
CONFIG_H264_MUXER=yes
CONFIG_IMAGE2_MUXER=yes
CONFIG_IMAGE2PIPE_MUXER=yes
CONFIG_IPOD_MUXER=yes
CONFIG_M4V_MUXER=yes
CONFIG_MATROSKA_MUXER=yes
CONFIG_MATROSKA_AUDIO_MUXER=yes
CONFIG_MJPEG_MUXER=yes
CONFIG_MMF_MUXER=yes
CONFIG_MOV_MUXER=yes
CONFIG_MP2_MUXER=yes
CONFIG_MP3_MUXER=yes
CONFIG_MP4_MUXER=yes
CONFIG_MPEG1SYSTEM_MUXER=yes
CONFIG_MPEG1VCD_MUXER=yes
CONFIG_MPEG1VIDEO_MUXER=yes
CONFIG_MPEG2DVD_MUXER=yes
CONFIG_MPEG2SVCD_MUXER=yes
CONFIG_MPEG2VIDEO_MUXER=yes
CONFIG_MPEG2VOB_MUXER=yes
CONFIG_MPEGTS_MUXER=yes
CONFIG_MPJPEG_MUXER=yes
CONFIG_NULL_MUXER=yes
CONFIG_NUT_MUXER=yes
CONFIG_OGG_MUXER=yes
CONFIG_PCM_ALAW_MUXER=yes
CONFIG_PCM_MULAW_MUXER=yes
CONFIG_PCM_S16BE_MUXER=yes
CONFIG_PCM_S16LE_MUXER=yes
CONFIG_PCM_S8_MUXER=yes
CONFIG_PCM_U16BE_MUXER=yes
CONFIG_PCM_U16LE_MUXER=yes
CONFIG_PCM_U8_MUXER=yes
CONFIG_PSP_MUXER=yes
CONFIG_RAWVIDEO_MUXER=yes
CONFIG_RM_MUXER=yes
CONFIG_ROQ_MUXER=yes
CONFIG_RTP_MUXER=yes
CONFIG_SWF_MUXER=yes
CONFIG_TG2_MUXER=yes
CONFIG_TGP_MUXER=yes
CONFIG_VOC_MUXER=yes
CONFIG_WAV_MUXER=yes
CONFIG_YUV4MPEGPIPE_MUXER=yes
CONFIG_CROP_FILTER=yes
CONFIG_FILE_PROTOCOL=yes
CONFIG_HTTP_PROTOCOL=yes
CONFIG_PIPE_PROTOCOL=yes
CONFIG_RTP_PROTOCOL=yes
CONFIG_TCP_PROTOCOL=yes
CONFIG_UDP_PROTOCOL=yes
CONFIG_DV1394_DEMUXER=yes
CONFIG_OSS_DEMUXER=yes
CONFIG_V4L2_DEMUXER=yes
CONFIG_V4L_DEMUXER=yes
CONFIG_OSS_MUXER=yes
SRC_PATH="/home/zenko/Internets/xcLaserTag/LaserTag2002Xcode/libs/fobs/resources/ffmpeg"
SRC_PATH_BARE=/home/zenko/Internets/xcLaserTag/LaserTag2002Xcode/libs/fobs/resources/ffmpeg
BUILD_ROOT="/home/zenko/Internets/xcLaserTag/LaserTag2002Xcode/libs/fobs/resources/ffmpeg"
