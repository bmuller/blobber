INCLUDES = ${GTK_CFLAGS} -I${top_srcdir}/blobber
AM_LDFLAGS = ${GTK_LIBS} -lblobber -L${top_srcdir}/blobber
AM_CPPFLAGS = -DLIBDIR='"$(libdir)/blobber"' -DDATAROOTDIR='"$(datarootdir)/blobber"'


# Modules that are always compiled 
pkglib_LTLIBRARIES = liblasertag.la libprojectionoptions.la
nobase_dist_pkgdata_DATA = 

liblasertag_la_SOURCES = laser_tag/laser_tag.cpp
libprojectionoptions_la_SOURCES = projection_options/projection_options.cpp


# Modules that are conditionally compiled
if PONG
  pkglib_LTLIBRARIES += libpong.la 
  libpong_la_SOURCES = pong/pong.cpp
endif

if INVADERS
  pkglib_LTLIBRARIES += libinvaders.la 
  libinvaders_la_SOURCES = invaders/invaders.cpp
  nobase_dist_pkgdata_DATA += invaders/invader0.png invaders/invader1.png \
                              invaders/invaderblk0.png invaders/invaderblk1.png \
                              invaders/pause.png invaders/kill.png invaders/killblk.png
endif

if FLOWER
  pkglib_LTLIBRARIES += libflower.la 
  libflower_la_SOURCES = flower/flower.cpp
  nobase_dist_pkgdata_DATA += flower/flower1.png flower/flower2.png flower/flower3.png \
                              flower/flower4.png flower/flower5.png flower/flower6.png \
                              flower/flower7.png flower/flower8.png flower/flower9.png \
                              flower/flower10.png
endif

if MULTILASERTAG
  pkglib_LTLIBRARIES += libmultilasertag.la 
  libmultilasertag_la_SOURCES = multi_laser_tag/multi_laser_tag.cpp
endif

if COLORRANDOMIZER
  pkglib_LTLIBRARIES += libcolorrandomizer.la
  libcolorrandomizer_la_SOURCES = color_randomizer/color_randomizer.cpp
endif

if SHAPESNSOUNDS
  pkglib_LTLIBRARIES += libshapesnsounds.la
  stkdir = libs/stk
  libshapesnsounds_la_SOURCES = shapesnsounds/shapesnsounds.cpp ${stkdir}/ADSR.cpp ${stkdir}/FileRead.cpp ${stkdir}/Filter.cpp \
	${stkdir}/Generator.cpp ${stkdir}/SineWave.cpp ${stkdir}/TwoZero.cpp ${stkdir}/WvIn.cpp ${stkdir}/Envelope.cpp \
	${stkdir}/FileWvIn.cpp ${stkdir}/FM.cpp ${stkdir}/Instrmnt.cpp ${stkdir}/RtAudio.cpp ${stkdir}/Stk.cpp \
	${stkdir}/WaveLoop.cpp ${stkdir}/BeeThree.cpp
  libshapesnsounds_la_CPPFLAGS = ${AM_CPPFLAGS} -I${stkdir} -D__GXX__ -D__LITTLE_ENDIAN__  -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__ 
  libshapesnsounds_la_LDFLAGS = ${AM_CPPFLAGS} -lasound
  nobase_dist_pkgdata_DATA += ${stkdir}/fwavblnk.raw ${stkdir}/sinewave.raw
endif

if POINTER
  pkglib_LTLIBRARIES += libpointer.la
  libpointer_la_SOURCES = pointer/pointer.cpp
  libpointer_la_LDFLAGS = ${AM_CPPFLAGS} -lX11 -lXtst
endif
