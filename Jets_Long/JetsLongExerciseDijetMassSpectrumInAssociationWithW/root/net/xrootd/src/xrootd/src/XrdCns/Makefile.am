#######################################################################
## Makefile.am for xrootd
##
##
## Initial version: 17.5.2008
##
## Version info: $Id: Makefile.am 38011 2011-02-08 18:35:57Z ganis $
## Checked in by $Author$
#######################################################################

#############################################################################
## extra directory definitions
xrootdcnsincdir = $(includedir)/xrootd/XrdCns

#############################################################################
## deliverables
bin_PROGRAMS = XrdCnsd cns_ssi

INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

xrootdcnsinc_HEADERS = XrdCnsConfig.hh \
                       XrdCnsDaemon.hh \
                       XrdCnsInventory.hh \
                       XrdCnsLog.hh \
                       XrdCnsLogClient.hh \
                       XrdCnsLogFile.hh \
                       XrdCnsLogRec.hh \
                       XrdCnsLogServer.hh \
                       XrdCnsSsi.hh \
                       XrdCnsSsiCfg.hh \
                       XrdCnsSsiSay.hh \
                       XrdCnsXref.hh

#############################################################################
## Build rules

XrdCnsd_SOURCES = \
                  XrdCnsConfig.cc \
                  XrdCnsDaemon.cc \
                  XrdCnsInventory.cc  \
                  XrdCnsLogClient.cc  \
                  XrdCnsLogFile.cc  \
                  XrdCnsLogServer.cc  \
                  XrdCnsMain.cc \
                  XrdCnsLog.cc  \
                  XrdCnsLogRec.cc \
                  XrdCnsXref.cc

XrdCnsd_LDADD   = \
                  ../Xrd/libXrd.la        \
                  ../XrdOss/libXrdOss.la     \
                  ../XrdOuc/libXrdOuc.la     \
                  ../XrdNet/libXrdNet.la     \
                  ../XrdSys/libXrdSys.la \
                  ../XrdClient/libXrdClient.la

XrdCnsd_LDFLAGS = -static -export-dynamic

$(OBJECT_XRDCNSD) $(BINLIBS) $(LIBM) $(LIBS)

cns_ssi_SOURCES = \
                  XrdCnsLog.cc  \
                  XrdCnsLogRec.cc \
                  XrdCnsXref.cc \
                  XrdCnsSsi.cc \
                  XrdCnsSsiCfg.cc \
                  XrdCnsSsiMain.cc

cns_ssi_LDADD   = \
                  ../Xrd/libXrd.la        \
                  ../XrdOss/libXrdOss.la     \
                  ../XrdOuc/libXrdOuc.la     \
                  ../XrdNet/libXrdNet.la     \
                  ../XrdSys/libXrdSys.la \
                  ../XrdClient/libXrdClient.la

cns_ssi_LDFLAGS = -static -export-dynamic

$(OBJECT_CNS_SSI) $(BINLIBS) $(LIBM) $(LIBS)
