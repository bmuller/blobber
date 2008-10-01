INCLUDES = ${GTK_CFLAGS}
AM_LDFLAGS = ${GTK_LIBS}
AM_CPPFLAGS = -DLIBDIR='"$(libdir)/blobber"' -DDATAROOTDIR='"$(datarootdir)/blobber"'


lib_LTLIBRARIES = libblobber.la
libblobber_la_SOURCES = camarea.cpp frame.cpp options_window.cpp framegrabber_factory.cpp \
	mod_interface.cpp projection_window.cpp utils.cpp camera_window.cpp pixconverter.cpp \
	configuration.cpp mod_utils/movable_object.cpp mod_utils/movable_square.cpp \
	mod_utils/movable_circle.cpp mod_utils/movable_crosshairs.cpp

if HAVE_V4L1
libblobber_la_SOURCES += framegrabberone.cpp
endif

if HAVE_V4L2
libblobber_la_SOURCES += framegrabbertwo.cpp
endif


bin_PROGRAMS = blobber
blobber_SOURCES = blobber.cpp
blobber_LDFLAGS = -lblobber