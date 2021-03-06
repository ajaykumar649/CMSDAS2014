// @(#)root/base:$Id: TException.cxx 22729 2008-03-19 10:20:10Z pcanal $
// Author: Fons Rademakers   21/09/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TException
#define ROOT_TException


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Exception Handling                                                   //
//                                                                      //
// Provide some macro's to simulate the coming C++ try, catch and throw //
// exception handling functionality.                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include <setjmp.h>
#else
struct sigjmp_buf;
struct jmp_buf;
#endif

struct ExceptionContext_t {
#ifdef NEED_SIGJMP
   sigjmp_buf fBuf;
#else
   jmp_buf fBuf;
#endif
};

#ifdef NEED_SIGJMP
#define SETJMP(buf) sigsetjmp(buf,1)
#else
#define SETJMP(buf) setjmp(buf)
#endif

#define RETRY \
   { \
      ExceptionContext_t R__curr, *R__old = gException; \
      int R__code; \
      gException = &R__curr; \
      R__code = SETJMP(gException->fBuf); if (R__code) { }; {

#define TRY \
   { \
      ExceptionContext_t R__curr, *R__old = gException; \
      int R__code; \
      gException = &R__curr; \
      if ((R__code = SETJMP(gException->fBuf)) == 0) {

#define CATCH(n) \
         gException = R__old; \
      } else { \
         int n = R__code; \
         gException = R__old;

#define ENDTRY \
      } \
      gException = R__old; \
   }

#define SOMETHING_TODO \
   { \
     int a; \
     a = a + 1; \
   }

extern ExceptionContext_t *gException;

extern void Throw(int code);

int main() {
   SOMETHING_TODO;
   return 0;
}
#endif
