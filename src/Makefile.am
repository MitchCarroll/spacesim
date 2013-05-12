AM_CPPFLAGS = -DDATADIR='"$(pkgdatadir)/"'
AM_CXXFLAGS = -I$(top_builddir) -I$(top_builddir)/src -I$(top_builddir)/vtx -I$(top_builddir)/quat
bin_PROGRAMS = spacesim
spacesim_SOURCES = main.cpp body.cpp body.h
spacesim_LDADD = $(top_builddir)/vtx/libvtx.a $(top_builddir)/quat/libquat.a $(AM_LDFLAGS)
spacesim_LIBS = "-lglut -lGLU -lGL"
