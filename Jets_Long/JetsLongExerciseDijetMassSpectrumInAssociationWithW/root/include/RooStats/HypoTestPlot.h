// @(#)root/roostats:$Id: HypoTestPlot.h 36602 2010-11-11 16:52:13Z moneta $
// Author: Sven Kreiss    June 2010
/*************************************************************************
 * Copyright (C) 1995-2008, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOSTATS_HypoTestPlot
#define ROOSTATS_HypoTestPlot

#ifndef ROOSTATS_SamplingDistPlot
#include "RooStats/SamplingDistPlot.h"
#endif

#ifndef ROOSTATS_SamplingDistribution
#include "RooStats/SamplingDistribution.h"
#endif

#ifndef ROOSTATS_HypoTestResult
#include "RooStats/HypoTestResult.h"
#endif

namespace RooStats {

class HypoTestPlot: public SamplingDistPlot {
   public:
      /// Constructor
      HypoTestPlot(HypoTestResult& result, Int_t bins=100);
      ~HypoTestPlot(void) {}

      // Applies a HypoTestResult.
      void ApplyResult(HypoTestResult& result);
      // Set default style options (also called in the constructor that takes a HypoTestResult).
      void ApplyDefaultStyle(void);

   private:
      HypoTestResult *fHypoTestResult;

   protected:
   ClassDef(HypoTestPlot,1)
};
}

#endif

