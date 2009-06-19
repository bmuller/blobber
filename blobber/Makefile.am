INCLUDES = ${GTK_CFLAGS}
AM_LDFLAGS = ${GTK_LIBS}
AM_CPPFLAGS = -DLIBDIR='"$(libdir)/blobber"' -DDATAROOTDIR='"$(datarootdir)/blobber"'


lib_LTLIBRARIES = libblobber.la
libblobber_la_SOURCES = camarea.cpp frame.cpp framegrabber_factory.cpp configuration.cpp \
	mod_interface.cpp projection_window.cpp utils.cpp camera_window.cpp colormap.cpp \
	application.cpp 

# modutils
libblobber_la_SOURCES += mod_utils/movable_object.cpp mod_utils/movable_square.cpp \
	mod_utils/movable_circle.cpp mod_utils/movable_crosshairs.cpp mod_utils/sprite.cpp \
	mod_utils/movable_box.cpp help_window.cpp 

# gtk utils
libblobber_la_SOURCES += gtk_utils/text_box.cpp

# options window
opwin = options_window
libblobber_la_SOURCES += ${opwin}/additional_options_tab.cpp ${opwin}/camera_options_tab.cpp\
	${opwin}/module_options_tab.cpp ${opwin}/poi_options_tab.cpp ${opwin}/options_window.cpp

if HAVE_V4L1
libblobber_la_SOURCES += framegrabberone.cpp
endif

#framegrabbertwo now default
libblobber_la_SOURCES += framegrabbertwo.cpp

bin_PROGRAMS = blobber
blobber_SOURCES = blobber.cpp
blobber_LDFLAGS = -lblobber
