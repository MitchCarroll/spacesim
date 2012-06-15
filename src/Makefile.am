AM_CXXFLAGS = "-I$(top_srcdir)/src $AM_CXXFLAGS"
bin_PROGRAMS = spacesim
spacesim_SOURCES = main.cpp
spacesim_LDADD = $(top_builddir)/vtx/libvtx.a -lglut -lGLU -lGL $(AM_LDFLAGS)
