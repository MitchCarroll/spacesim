AM_CXXFLAGS = "-I$(top_srcdir) -I$(top_srcdir)/src -I$(top_srcdir)/vtx $AM_CXXFLAGS"
bin_PROGRAMS = spacesim
spacesim_SOURCES = main.cpp
spacesim_LDADD = $(top_srcdir)/vtx/libvtx.a -lglut -lGLU -lGL $(AM_LDFLAGS)
