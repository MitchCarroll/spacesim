AM_CXXFLAGS = "-I$(top_srcdir) -I$(top_srcdir)/src -I$(top_srcdir)/vtx -I$(top_srcdir)/quat $AM_CXXFLAGS"
bin_PROGRAMS = spacesim
spacesim_SOURCES = main.cpp
spacesim_LDADD = $(top_srcdir)/vtx/libvtx.a $(top_srcdir)/quat/libquat.a -lglut -lGLU -lGL $(AM_LDFLAGS)
