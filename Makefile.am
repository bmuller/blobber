SUBDIRS = blobber modules

HEADERDIR = blobber
nobase_include_HEADERS = $(HEADERDIR)/blobber.h $(HEADERDIR)/camera_window.h $(HEADERDIR)/exception.h \
			 $(HEADERDIR)/frame.h $(HEADERDIR)/options_window.h $(HEADERDIR)/projection_window.h \
			 $(HEADERDIR)/utils.h $(HEADERDIR)/camarea.h $(HEADERDIR)/config.h $(HEADERDIR)/types.h \
			 $(HEADERDIR)/framegrabber.h $(HEADERDIR)/mod_interface.h $(HEADERDIR)/pixconverter.h

dist_pkgdata_DATA = blobber/nocam.png

install-exec-hook:
	ldconfig