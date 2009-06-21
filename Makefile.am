SUBDIRS = blobber modules

ACLOCAL_AMFLAGS=-I m4

pkgconfigdir=${libdir}/pkgconfig
pkgconfig_DATA=libblobber.pc

HEADERDIR = blobber
MODUTILSDIR = $(HEADERDIR)/mod_utils
OPTIONSDIR = $(HEADERDIR)/options_window
GTKUTILSDIR = $(HEADERDIR)/gtk_utils
nobase_include_HEADERS = $(HEADERDIR)/blobber.h $(HEADERDIR)/camera_window.h $(HEADERDIR)/exception.h \
			 $(HEADERDIR)/frame.h $(HEADERDIR)/projection_window.h $(MODUTILSDIR)/sprite.h \
			 $(HEADERDIR)/utils.h $(HEADERDIR)/camarea.h $(HEADERDIR)/config.h $(HEADERDIR)/types.h \
			 $(HEADERDIR)/framegrabber.h $(HEADERDIR)/mod_interface.h $(HEADERDIR)/colormap.h \
			 $(HEADERDIR)/configuration.h $(HEADERDIR)/application.h $(MODUTILSDIR)/movable_object.h \
			 $(MODUTILSDIR)/movable_circle.h $(MODUTILSDIR)/movable_square.h \
			 $(MODUTILSDIR)/movable_crosshairs.h $(OPTIONSDIR)/additional_options_tab.h \
	 		 $(OPTIONSDIR)/camera_options_tab.h $(OPTIONSDIR)/module_options_tab.h \
			 $(OPTIONSDIR)/options_window.h $(OPTIONSDIR)/poi_options_tab.h $(HEADERDIR)/help_window.h \
			 $(GTKUTILSDIR)/text_box.h

dist_pkgdata_DATA = blobber/nocam.png blobber/blobber.png

install-exec-hook:
	/sbin/ldconfig

if HAVE_DOXYGEN
dox: Doxyfile
	${DOXYGEN} && echo && echo "You can now view your documentation by browsing to the apidocs/html directory and viewing index.html in a browser."
clean-local:
	rm -rf apidocs
else
dox:
	echo "You need to configure with --enable-doxygen if you want to get the docs too."
endif
