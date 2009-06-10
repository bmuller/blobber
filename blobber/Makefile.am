INCLUDES = ${GTK_CFLAGS}
AM_LDFLAGS = ${GTK_LIBS}
AM_CPPFLAGS = -DLIBDIR='"$(libdir)/blobber"' -DDATAROOTDIR='"$(datarootdir)/blobber"'


lib_LTLIBRARIES = libblobber.la
libblobber_la_SOURCES = camarea.cpp frame.cpp options_window.cpp framegrabber_factory.cpp \
	mod_interface.cpp projection_window.cpp utils.cpp camera_window.cpp colormap.cpp \
	application.cpp configuration.cpp mod_utils/movable_object.cpp mod_utils/movable_square.cpp \
	mod_utils/movable_circle.cpp mod_utils/movable_crosshairs.cpp mod_utils/sprite.cpp 

if HAVE_V4L1
libblobber_la_SOURCES += framegrabberone.cpp
endif

#framegrabbertwo now default
libblobber_la_SOURCES += framegrabbertwo.cpp

bin_PROGRAMS = blobber
blobber_SOURCES = blobber.cpp
blobber_LDFLAGS = -lblobber
