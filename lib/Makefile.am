#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libCPPMisc.la

#######################################
# libCPPMisc options
#######################################
libCPPMisc_la_SOURCES = CmdOptionParser.cpp \
                        SpookyV2.cpp

libCPPMisc_la_LDFLAGS = -version-info 1:0:0

libCPPMisc_la_CPPFLAGS = -I$(top_srcdir)/include
