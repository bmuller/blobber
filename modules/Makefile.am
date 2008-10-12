INCLUDES = ${GTK_CFLAGS} -I${top_srcdir}/blobber
AM_LDFLAGS = ${GTK_LIBS} -lblobber -L${top_srcdir}/blobber
AM_CPPFLAGS = -DLIBDIR='"$(libdir)/blobber"' -DDATAROOTDIR='"$(datarootdir)/blobber"'

pkglib_LTLIBRARIES = libgreenscreen.la liblasertag.la libprojectionoptions.la \
		     libmultilasertag.la libmulticoloredtag.la libpong.la \
		     libcolorrandomizer.la libinvaders.la librefreshrate.la \
		     libmovableshapes.la libflower.la

libgreenscreen_la_SOURCES = green_screen/green_screen.cpp
nobase_dist_pkgdata_DATA = green_screen/stone-640x480.jpg

liblasertag_la_SOURCES = laser_tag/laser_tag.cpp

libprojectionoptions_la_SOURCES = projection_options/projection_options.cpp

libmultilasertag_la_SOURCES = multi_laser_tag/multi_laser_tag.cpp

libmulticoloredtag_la_SOURCES = multi_colored_tag/multi_colored_tag.cpp

libpong_la_SOURCES = pong/pong.cpp pong/ball.cpp pong/paddle.cpp

libcolorrandomizer_la_SOURCES = color_randomizer/color_randomizer.cpp

libinvaders_la_SOURCES = invaders/invaders.cpp
nobase_dist_pkgdata_DATA += invaders/ghost1.png invaders/ghost2.png invaders/ghost3.png \
                           invaders/ghost4.png invaders/ghost5.png invaders/ghost6.png \
                           invaders/ghost7.png invaders/ghost8.png invaders/ghost9.png \
                           invaders/explode1.png invaders/explode2.png \
                           invaders/explode3.png invaders/explode4.png invaders/explode5.png \
                           invaders/explode6.png invaders/explode7.png invaders/explode8.png \
                           invaders/pause.png invaders/cross.png

libflower_la_SOURCES = flower/flower.cpp
nobase_dist_pkgdata_DATA += flower/flower1.png flower/flower2.png flower/flower3.png \
                            flower/flower4.png flower/flower5.png flower/flower6.png \
                            flower/flower7.png flower/flower8.png flower/flower9.png

librefreshrate_la_SOURCES = refresh_rate/refresh_rate.cpp

libmovableshapes_la_SOURCES = movable_shapes/movable_shapes.cpp
