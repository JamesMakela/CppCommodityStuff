#######################################
# The list of executables we are building seperated by spaces
# A 'bin_' prefix indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
#
# The 'noinst_' prefix indicates that the following targets are to be built,
# but not installed.
bin_PROGRAMS=csv_file

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

ACLOCAL_AMFLAGS=-I ../m4

# Sources for the a.out 
csv_file_SOURCES= CSVFile.cpp

# Libraries for a.out
csv_file_LDADD = $(top_srcdir)/lib/libCPPMisc.la \
                 $(top_srcdir)/lib/libCSVFile.la

# Linker options for a.out
csv_file_LDFLAGS = -rpath `cd $(top_srcdir);pwd`/lib/.libs

# Compiler options for a.out
csv_file_CPPFLAGS = -I$(top_srcdir)/include
