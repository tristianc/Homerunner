#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libhomerunner.la

#######################################
# Build information for each library

# Make resources
homerunner.gresource.c: homerunner.gresource.xml tj_channel_list.ui
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir) --generate-source homerunner.gresource.xml

# Make marshalling functions
marshaller.h: marshaller.list
	$(GLIB_GENMARSHAL) --header marshaller.list > marshaller.h
marshaller.c: marshaller.h marshaller.list
	$(GLIB_GENMARSHAL) --body marshaller.list > marshaller.c

# Sources for homerunner
libhomerunner_la_SOURCES = homerunner_plugin.c marshaller.c tj_channel_list.c tj_hdhr_manager.c tj_hdhr_channel_manager.c tj_hdhr_playback_manager.c homerunner.gresource.c

# Linker options libhomerunner
libhomerunner_la_LDFLAGS = $(GLIB_LIBS) $(TOTEM_LIBS) $(LIBPEAS_LIBS) $(LIBXML2_LIBS) $(HDHOMERUN_LIBS) $(CLUTTERGTK_LIBS)

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libhomerunner_la_CFLAGS = -O0 -I$(top_srcdir)/include $(GLIB_CFLAGS) $(TOTEM_CFLAGS) $(LIBPEAS_CFLAGS) $(LIBXML2_CFLAGS) $(HDHOMERUN_CFLAGS) $(CLUTTERGTK_CFLAGS)

clean-local:
	rm -rf homerunner.gresource.c marshaller.c marshaller.h
