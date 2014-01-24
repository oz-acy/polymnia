# Makefile.am for POLYMNIA

AM_CFLAGS = -O2
AM_CXXFLAGS = -O2

lib_LIBRARIES = libpolymnia.a

libpolymnia_a_SOURCES = \
	ibuf.h ibuf-imp_.h ibuf.cpp \
	rgb.h picture.h picture.cpp \
	pictio.h \
	dibio.h dibin.cpp dibout.cpp \
	jpegio.h jpegio.cpp jpegin.cpp jpegout.cpp \
	pngio.h pngin.cpp pngout.cpp \
	pictcvt.h \
	colcnv.cpp toindex.cpp tograyscale.cpp \
	reduce.cpp resize.cpp


polymniaincludedir = @includedir@/polymnia

polymniainclude_HEADERS = \
	ibuf.h ibuf-imp_.h rgb.h picture.h \
	pictio.h \
	dibio.h jpegio.h pngio.h \
	pictcvt.h


CLEANFILES = *~

#eof
