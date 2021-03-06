// @(#)root/auth:$Id: TRootSecContext.h 20882 2007-11-19 11:31:26Z rdm $
// Author: G. Ganis   08/07/2005

/*************************************************************************
 * Copyright (C) 1995-2005, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TRootSecContext
#define ROOT_TRootSecContext


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRootSecContext                                                      //
//                                                                      //
// Special implementation of TSecContext                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TAuthenticate
#include "TAuthenticate.h"
#endif
#ifndef ROOT_TSecContext
#include "TSecContext.h"
#endif

class TRootSecContext : public TSecContext {

private:
   Int_t        fRSAKey;              // Type of RSA key used

   Bool_t       CleanupSecContext(Bool_t all);

public:

   TRootSecContext(const char *url, Int_t meth, Int_t offset,
               const char *id, const char *token,
               TDatime expdate = kROOTTZERO, void *ctx = 0, Int_t key = 1);
   TRootSecContext(const char *user, const char *host, Int_t meth, Int_t offset,
               const char *id, const char *token,
               TDatime expdate = kROOTTZERO, void *ctx = 0, Int_t key = 1);
   virtual    ~TRootSecContext();

   const char *AsString(TString &out);

   void        DeActivate(Option_t *opt = "CR");
   Int_t       GetRSAKey()  const { return fRSAKey; }

   void        Print(Option_t *option = "F") const;

   ClassDef(TRootSecContext,0)  // Class providing host specific authentication information
};



#endif
