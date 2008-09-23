INCLUDES = ${GTK_CFLAGS} -I${top_srcdir}/blobber
AM_LDFLAGS = ${GTK_LIBS} -lblobber -L${top_srcdir}/blobber
AM_CPPFLAGS = -DLIBDIR='"$(libdir)/blobber"' -DDATAROOTDIR='"$(datarootdir)/blobber"'

pkglib_LTLIBRARIES = libgreenscreen.la liblasertag.la libprojectionoptions.la \
		     libmultilasertag.la libmulticoloredtag.la libpong.la \
		     libcolorrandomizer.la

libgreenscreen_la_SOURCES = green_screen/green_screen.cpp
nobase_dist_pkgdata_DATA = green_screen/stone-640x480.jpg

liblasertag_la_SOURCES = laser_tag/laser_tag.cpp

libprojectionoptions_la_SOURCES = projection_options/projection_options.cpp

libmultilasertag_la_SOURCES = multi_laser_tag/multi_laser_tag.cpp

libmulticoloredtag_la_SOURCES = multi_colored_tag/multi_colored_tag.cpp

libpong_la_SOURCES = pong/pong.cpp

libcolorrandomizer_la_SOURCES = color_randomizer/color_randomizer.cpp

