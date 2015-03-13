AUTOMAKE_OPTIONS = foreign
AM_CFLAGS = --pedantic -Wall
if BUILD_STATIC
bin_PROGRAMS = pdfpreview_dynamic pdfpreview_static
else
bin_PROGRAMS = pdfpreview_dynamic
endif
pdfpreview_dynamic_SOURCES = pdfpreview.cpp
pdfpreview_dynamic_CPPFLAGS = $(AM_CPPFLAGS)
pdfpreview_dynamic_CXXFLAGS = $(POPPLER_CFLAGS) $(AM_CFLAGS)
pdfpreview_dynamic_LDADD = $(POPPLER_LIBS)


### This is an ugly hack
### You will probably need to fix the list of libraries for your system
pdfpreview_static_SOURCES = pdfpreview.cpp
pdfpreview_static_CPPFLAGS = $(AM_CPPFLAGS)
pdfpreview_static_CXXFLAGS = $(POPPLER_CFLAGS) $(AM_CFLAGS)
pdfpreview_static_LDFLAGS = -all-static
pdfpreview_static_LDADD = $(POPPLER_LIBS) -lfreetype -lfontconfig -lfreetype -lpthread -ltiff -ljpeg -lstdc++ -lm -lc -llzma -ljbig -lz -lexpat -lgcc -ldl -lpng12 -llcms2
