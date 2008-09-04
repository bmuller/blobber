INCLUDES = ${GTK_CFLAGS}
AM_LDFLAGS = ${GTK_LIBS}

bin_PROGRAMS = blobber

blobber_SOURCES = blobber.cpp camarea.cpp frame.cpp options_window.cpp framegrabber_factory.cpp \
	projection_window.cpp utils.cpp mods/laser_tag.cpp mods/green_screen.cpp camera_window.cpp pixconverter.cpp 

if HAVE_V4L1
blobber_SOURCES += framegrabberone.cpp
endif

if HAVE_V4L2
blobber_SOURCES += framegrabbertwo.cpp
endif
