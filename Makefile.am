SUBDIRS = blobber modules

ACLOCAL_AMFLAGS=-I m4

pkgconfigdir=${libdir}/pkgconfig
pkgconfig_DATA=libblobber.pc

HEADERDIR = blobber
MODUTILSDIR = $(HEADERDIR)/mod_utils
nobase_include_HEADERS = $(HEADERDIR)/blobber.h $(HEADERDIR)/camera_window.h $(HEADERDIR)/exception.h \
			 $(HEADERDIR)/frame.h $(HEADERDIR)/options_window.h $(HEADERDIR)/projection_window.h \
			 $(HEADERDIR)/utils.h $(HEADERDIR)/camarea.h $(HEADERDIR)/config.h $(HEADERDIR)/types.h \
			 $(HEADERDIR)/framegrabber.h $(HEADERDIR)/mod_interface.h $(HEADERDIR)/colormap.h \
			 $(HEADERDIR)/configuration.h $(HEADERDIR)/application.h $(MODUTILSDIR)/movable_object.h \
			 $(MODUTILSDIR)/movable_circle.h $(MODUTILSDIR)/movable_square.h \
			 $(MODUTILSDIR)/movable_crosshairs.h $(MODUTILSDIR)/sprite.h

dist_pkgdata_DATA = blobber/nocam.png blobber/blobber.png

install-exec-hook:
	ldconfig

if HAVE_DOXYGEN
dox: Doxyfile
	${DOXYGEN} && echo && echo "You can now view your documentation by browsing to the apidocs/html directory and viewing index.html in a browser."
clean-local:
	rm -rf apidocs
else
dox:
	echo "You need to configure with --enable-doxygen if you want to get the docs too."
endif
