#######################################################################
## Makefile.am for xrootd
##
##
## Initial version: 1.8.2005
##
## Version info: $Id: Makefile.am 38011 2011-02-08 18:35:57Z ganis $
## Checked in by $Author$
#######################################################################

include $(top_srcdir)/src/Makefile_include

# preprocessor flags
if ENABLE_FUSE
AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_builddir)/src -DHAVE_FUSE=1
FUSELIB=-lfuse
else
AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_builddir)/src
FUSELIB=
endif

#############################################################################
## extra directory definitions for install area
xrootdffsincdir = $(includedir)/xrootd/XrdFfs
xrootdffsinc_HEADERS =  XrdFfsDent.hh XrdFfsFsinfo.hh \
                        XrdFfsMisc.hh XrdFfsQueue.hh XrdFfsWcache.hh

#############################################################################
## deliverables

lib_LTLIBRARIES = libXrdFfs.la
bin_PROGRAMS = xrootdfs

#############################################################################
## Build rules

libXrdFfs_la_SOURCES = \
        XrdFfsDent.cc  \
        XrdFfsFsinfo.cc \
        XrdFfsMisc.cc \
        XrdFfsPosix.cc \
        XrdFfsQueue.cc \
        XrdFfsWcache.cc

libXrdFfs_la_LIBADD = ../XrdPosix/libXrdPosix.la \
                      ../XrdClient/libXrdClient.la \
                      ../XrdSec/libXrdSec.la \
                      ../XrdSecsss/libXrdSecsss.la \
                      ../XrdNet/libXrdNet.la \
                      ../XrdNet/libXrdNetUtil.la \
                      ../XrdOuc/libXrdOuc.la \
                      ../XrdSys/libXrdSys.la \
                      $(FUSELIB)

xrootdfs_SOURCES = XrdFfsXrootdfs.cc
xrootdfs_LDADD = libXrdFfs.la
