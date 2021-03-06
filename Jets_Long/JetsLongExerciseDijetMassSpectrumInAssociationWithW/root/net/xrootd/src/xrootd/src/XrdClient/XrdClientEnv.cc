//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdClientEnv                                                         // 
//                                                                      //
// Author: Fabrizio Furano (INFN Padova, 2004)                          //
// Adapted from TXNetFile (root.cern.ch) originally done by             //
//  Alvise Dorigo, Fabrizio Furano                                      //
//          INFN Padova, 2003                                           //
//                                                                      //
// Singleton used to handle the default parameter values                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "XrdSys/XrdSysHeaders.hh"
#include "XrdClient/XrdClientConst.hh"
#include "XrdClient/XrdClientEnv.hh"
#include <string>
#include <algorithm>
#include <ctype.h>

XrdClientEnv *XrdClientEnv::fgInstance = 0;

XrdClientEnv *XrdClientEnv::Instance() {
   // Create unique instance

   if (!fgInstance) {
      fgInstance = new XrdClientEnv;
      if (!fgInstance) {
	 std::cerr << "XrdClientEnv::Instance: fatal - couldn't create XrdClientEnv" << std::endl;
         abort();
      }
   }
   return fgInstance;
}

//_____________________________________________________________________________
XrdClientEnv::XrdClientEnv() {
   // Constructor
   fOucEnv   = new XrdOucEnv();
   fShellEnv = new XrdOucEnv();

   PutInt(NAME_CONNECTTIMEOUT, DFLT_CONNECTTIMEOUT);
   PutInt(NAME_REQUESTTIMEOUT, DFLT_REQUESTTIMEOUT);
   PutInt(NAME_MAXREDIRECTCOUNT, DFLT_MAXREDIRECTCOUNT);
   PutInt(NAME_DEBUG, DFLT_DEBUG);
   PutInt(NAME_RECONNECTWAIT, DFLT_RECONNECTWAIT);
   PutInt(NAME_REDIRCNTTIMEOUT, DFLT_REDIRCNTTIMEOUT);
   PutInt(NAME_FIRSTCONNECTMAXCNT, DFLT_FIRSTCONNECTMAXCNT);
   PutInt(NAME_READCACHESIZE, DFLT_READCACHESIZE);
   PutInt(NAME_READCACHEBLKREMPOLICY, DFLT_READCACHEBLKREMPOLICY);
   PutInt(NAME_READAHEADSIZE, DFLT_READAHEADSIZE);
   PutInt(NAME_MULTISTREAMCNT, DFLT_MULTISTREAMCNT);
   PutInt(NAME_DFLTTCPWINDOWSIZE, DFLT_DFLTTCPWINDOWSIZE);
   PutInt(NAME_DATASERVERCONN_TTL, DFLT_DATASERVERCONN_TTL);
   PutInt(NAME_LBSERVERCONN_TTL, DFLT_LBSERVERCONN_TTL);
   PutInt(NAME_PURGEWRITTENBLOCKS, DFLT_PURGEWRITTENBLOCKS);
   PutInt(NAME_READAHEADSTRATEGY, DFLT_READAHEADSTRATEGY);
   PutInt(NAME_READTRIMBLKSZ, DFLT_READTRIMBLKSZ);
   PutInt(NAME_TRANSACTIONTIMEOUT, DFLT_TRANSACTIONTIMEOUT);
   PutInt(NAME_REMUSEDCACHEBLKS, DFLT_REMUSEDCACHEBLKS);

   ImportInt( NAME_CONNECTTIMEOUT );
   ImportInt( NAME_REQUESTTIMEOUT );
   ImportInt( NAME_MAXREDIRECTCOUNT );
   ImportInt( NAME_DEBUG );
   ImportInt( NAME_RECONNECTWAIT );
   ImportInt( NAME_REDIRCNTTIMEOUT );
   ImportInt( NAME_FIRSTCONNECTMAXCNT );
   ImportInt( NAME_READCACHESIZE );
   ImportInt( NAME_READCACHEBLKREMPOLICY );
   ImportInt( NAME_READAHEADSIZE );
   ImportInt( NAME_MULTISTREAMCNT );
   ImportInt( NAME_DFLTTCPWINDOWSIZE );
   ImportInt( NAME_DATASERVERCONN_TTL );
   ImportInt( NAME_LBSERVERCONN_TTL );
   ImportInt( NAME_PURGEWRITTENBLOCKS );
   ImportInt( NAME_READAHEADSTRATEGY );
   ImportInt( NAME_READTRIMBLKSZ );
   ImportInt( NAME_TRANSACTIONTIMEOUT );
   ImportInt( NAME_REMUSEDCACHEBLKS );
}

//------------------------------------------------------------------------------
// Import a string variable from the shell environment
//------------------------------------------------------------------------------
bool XrdClientEnv::ImportStr( const char *varname )
{
  std::string name = "XRD_";
  name += varname;
  std::transform( name.begin(), name.end(), name.begin(), ::toupper );

  char *value;
  if( !XrdOucEnv::Import( name.c_str(), value ) )
     return false;

  fShellEnv->Put( varname, value );
  return true;
}

//------------------------------------------------------------------------------
// Import an int variable from the shell environment
//------------------------------------------------------------------------------
bool XrdClientEnv::ImportInt( const char *varname )
{
  std::string name = "XRD_";
  name += varname;
  std::transform( name.begin(), name.end(), name.begin(), ::toupper );

  long value;
  if( !XrdOucEnv::Import( name.c_str(), value ) )
     return false;

  fShellEnv->PutInt( varname, value );
  return true;
}

//------------------------------------------------------------------------------
// Get a string from the shell environment
//------------------------------------------------------------------------------
const char *XrdClientEnv::ShellGet(const char *varname)
{
  XrdSysMutexHelper m( fMutex );
  const char *res = fShellEnv->Get( varname );
  if( res )
    return res;

  res = fOucEnv->Get( varname );
  return res;
}

//------------------------------------------------------------------------------
// Get an integer from the shell environment
//------------------------------------------------------------------------------
long XrdClientEnv::ShellGetInt(const char *varname)
{
  XrdSysMutexHelper m( fMutex );
  const char *res = fShellEnv->Get( varname );

  if( res )
    return fShellEnv->GetInt( varname );

  return fOucEnv->GetInt( varname );
}


//_____________________________________________________________________________
XrdClientEnv::~XrdClientEnv() {
   // Destructor
   delete fOucEnv;
   delete fShellEnv;
   delete fgInstance;

   fgInstance = 0;
}
