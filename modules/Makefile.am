INCLUDES = ${GTK_CFLAGS} -I${top_srcdir}/blobber
AM_LDFLAGS = ${GTK_LIBS} -lblobber -L${top_srcdir}/blobber
AM_CPPFLAGS = -DLIBDIR='"$(libdir)/blobber"' -DDATAROOTDIR='"$(datarootdir)/blobber"'


# Modules that are always compiled 
pkglib_LTLIBRARIES = liblasertag.la libprojectionoptions.la
nobase_dist_pkgdata_DATA = 

liblasertag_la_SOURCES = laser_tag/laser_tag.cpp
libprojectionoptions_la_SOURCES = projection_options/projection_options.cpp


# Modules that are conditionally compiled
if GREENSCREEN
  pkglib_LTLIBRARIES += libgreenscreen.la 
  libgreenscreen_la_SOURCES = green_screen/green_screen.cpp
  nobase_dist_pkgdata_DATA += green_screen/stone-640x480.jpg
endif

if MULTICOLOREDTAG
  pkglib_LTLIBRARIES += libmulticoloredtag.la 
  libmulticoloredtag_la_SOURCES = multi_colored_tag/multi_colored_tag.cpp
endif

if PONG
  pkglib_LTLIBRARIES += libpong.la 
  libpong_la_SOURCES = pong/pong.cpp pong/ball.cpp pong/paddle.cpp
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

if REFRESHRATE
  pkglib_LTLIBRARIES += librefreshrate.la 
  librefreshrate_la_SOURCES = refresh_rate/refresh_rate.cpp
endif

if MOVABLESHAPES
  pkglib_LTLIBRARIES += libmovableshapes.la 
  libmovableshapes_la_SOURCES = movable_shapes/movable_shapes.cpp
endif

if MULTILASERTAG
  pkglib_LTLIBRARIES += libmultilasertag.la 
  libmultilasertag_la_SOURCES = multi_laser_tag/multi_laser_tag.cpp
endif

if COLORRANDOMIZER
  pkglib_LTLIBRARIES += libcolorrandomizer.la
  libcolorrandomizer_la_SOURCES = color_randomizer/color_randomizer.cpp
endif

if DUCKHUNT
  pkglib_LTLIBRARIES += libduckhunt.la
  libduckhunt_la_SOURCES = duckhunt/duckhunt.cpp
  nobase_dist_pkgdata_DATA += duckhunt/duck.png duckhunt/deadduck.png duckhunt/background.png
endif

if THEREMIN
  pkglib_LTLIBRARIES += libtheremin.la
  stkdir = libs/stk
  libtheremin_la_SOURCES = theremin/theremin.cpp ${stkdir}/ADSR.cpp ${stkdir}/FileRead.cpp ${stkdir}/Filter.cpp \
	${stkdir}/Generator.cpp ${stkdir}/SineWave.cpp ${stkdir}/TwoZero.cpp ${stkdir}/WvIn.cpp ${stkdir}/Envelope.cpp \
	${stkdir}/FileWvIn.cpp ${stkdir}/FM.cpp ${stkdir}/Instrmnt.cpp ${stkdir}/RtAudio.cpp ${stkdir}/Stk.cpp \
	${stkdir}/WaveLoop.cpp ${stkdir}/BeeThree.cpp
  libtheremin_la_CPPFLAGS = ${AM_CPPFLAGS} -I${stkdir} -D__GXX__ -D__LITTLE_ENDIAN__  -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__ 
  libtheremin_la_LDFLAGS = ${AM_CPPFLAGS} -lasound
  nobase_dist_pkgdata_DATA += ${stkdir}/fwavblnk.raw ${stkdir}/sinewave.raw
endif

if POINTER
  pkglib_LTLIBRARIES += libpointer.la
  libpointer_la_SOURCES = pointer/pointer.cpp
  libpointer_la_LDFLAGS = ${AM_CPPFLAGS} -lX11 -lXtst
endif
