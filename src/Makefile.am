AM_CXXFLAGS = "-I$(top_distdir) -I$(top_distdir)/src -I$(top_distdir)/vtx -I$(top_distdir)/quat $AM_CXXFLAGS"
bin_PROGRAMS = spacesim
spacesim_SOURCES = main.cpp body.cpp body.h
spacesim_LDADD = $(top_builddir)/vtx/libvtx.a $(top_builddir)/quat/libquat.a $(AM_LDFLAGS)
spacesim_LIBS = "-lglut -lGLU -lGL"
