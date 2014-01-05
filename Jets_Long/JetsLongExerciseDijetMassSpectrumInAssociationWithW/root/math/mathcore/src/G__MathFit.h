/********************************************************************
* math/mathcore/src/G__MathFit.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error math/mathcore/src/G__MathFit.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
#include "FastAllocString.h"
extern "C" {
extern void G__cpp_setup_tagtableG__MathFit();
extern void G__cpp_setup_inheritanceG__MathFit();
extern void G__cpp_setup_typetableG__MathFit();
extern void G__cpp_setup_memvarG__MathFit();
extern void G__cpp_setup_globalG__MathFit();
extern void G__cpp_setup_memfuncG__MathFit();
extern void G__cpp_setup_funcG__MathFit();
extern void G__set_cpp_environmentG__MathFit();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "Fit/BinData.h"
#include "Fit/BinPoint.h"
#include "Fit/Chi2FCN.h"
#include "Fit/DataOptions.h"
#include "Fit/DataRange.h"
#include "Fit/DataVector.h"
#include "Fit/DataVectorfwd.h"
#include "Fit/FcnAdapter.h"
#include "Fit/FitConfig.h"
#include "Fit/FitResult.h"
#include "Fit/FitUtil.h"
#include "Fit/FitUtilParallel.h"
#include "Fit/Fitter.h"
#include "Fit/LogLikelihoodFCN.h"
#include "Fit/ParameterSettings.h"
#include "Fit/PoissonLikelihoodFCN.h"
#include "Fit/SparseData.h"
#include "Fit/UnBinData.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__G__MathFitLN_ROOT;
extern G__linked_taginfo G__G__MathFitLN_basic_ostreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__MathFitLN_string;
extern G__linked_taginfo G__G__MathFitLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__G__MathFitLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MathFitLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__G__MathFitLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFit;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLDataOptions;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLDataRange;
extern G__linked_taginfo G__G__MathFitLN_pairlEdoublecOdoublegR;
extern G__linked_taginfo G__G__MathFitLN_vectorlEpairlEdoublecOdoublegRcOallocatorlEpairlEdoublecOdoublegRsPgRsPgR;
extern G__linked_taginfo G__G__MathFitLN_reverse_iteratorlEvectorlEpairlEdoublecOdoublegRcOallocatorlEpairlEdoublecOdoublegRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MathFitLN_vectorlEvectorlEpairlEdoublecOdoublegRcOallocatorlEpairlEdoublecOdoublegRsPgRsPgRcOallocatorlEvectorlEpairlEdoublecOdoublegRcOallocatorlEpairlEdoublecOdoublegRsPgRsPgRsPgRsPgR;
extern G__linked_taginfo G__G__MathFitLN_reverse_iteratorlEvectorlEvectorlEpairlEdoublecOdoublegRcOallocatorlEpairlEdoublecOdoublegRsPgRsPgRcOallocatorlEvectorlEpairlEdoublecOdoublegRcOallocatorlEpairlEdoublecOdoublegRsPgRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLFitData;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLDataVector;
extern G__linked_taginfo G__G__MathFitLN_vectorlEdoublecOallocatorlEdoublegRsPgR;
extern G__linked_taginfo G__G__MathFitLN_reverse_iteratorlEvectorlEdoublecOallocatorlEdoublegRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLDataWrapper;
extern G__linked_taginfo G__G__MathFitLN_vectorlEconstsPdoublemUcOallocatorlEconstsPdoublemUgRsPgR;
extern G__linked_taginfo G__G__MathFitLN_reverse_iteratorlEvectorlEconstsPdoublemUcOallocatorlEconstsPdoublemUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLBinData;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLBinDatacLcLErrorType;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMath;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLIBaseFunctionMultiDim;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLIGradientFunctionMultiDim;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLIGradientMultiDim;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLBasicFitMethodFunctionlEROOTcLcLMathcLcLIBaseFunctionMultiDimgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLBasicFitMethodFunctionlEROOTcLcLMathcLcLIBaseFunctionMultiDimgRcLcLType_t;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLBasicFitMethodFunctionlEROOTcLcLMathcLcLIGradientFunctionMultiDimgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLBasicFitMethodFunctionlEROOTcLcLMathcLcLIGradientFunctionMultiDimgRcLcLType_t;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLIParametricFunctionOneDim;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLIParametricGradFunctionOneDim;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLIParametricFunctionMultiDim;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLIParametricGradFunctionMultiDim;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLUnBinData;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLChi2FCNlEROOTcLcLMathcLcLIBaseFunctionMultiDimgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLChi2FCNlEROOTcLcLMathcLcLIGradientFunctionMultiDimgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLParameterSettings;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLMinimizerOptions;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLMathcLcLMinimizer;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLFitConfig;
extern G__linked_taginfo G__G__MathFitLN_vectorlEROOTcLcLFitcLcLParameterSettingscOallocatorlEROOTcLcLFitcLcLParameterSettingsgRsPgR;
extern G__linked_taginfo G__G__MathFitLN_vectorlEROOTcLcLFitcLcLParameterSettingscOallocatorlEROOTcLcLFitcLcLParameterSettingsgRsPgRcLcLiterator;
extern G__linked_taginfo G__G__MathFitLN_reverse_iteratorlEvectorlEROOTcLcLFitcLcLParameterSettingscOallocatorlEROOTcLcLFitcLcLParameterSettingsgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MathFitLN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR;
extern G__linked_taginfo G__G__MathFitLN_reverse_iteratorlEvectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLFitResult;
extern G__linked_taginfo G__G__MathFitLN_maplEunsignedsPintcOpairlEdoublecOdoublegRcOlesslEunsignedsPintgRcOallocatorlEpairlEconstsPunsignedsPintcOpairlEdoublecOdoublegRsPgRsPgRsPgR;
extern G__linked_taginfo G__G__MathFitLN_vectorlEstringcOallocatorlEstringgRsPgR;
extern G__linked_taginfo G__G__MathFitLN_reverse_iteratorlEvectorlEstringcOallocatorlEstringgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLFitter;
extern G__linked_taginfo G__G__MathFitLN_auto_ptrlEROOTcLcLFitcLcLFitResultgR;
extern G__linked_taginfo G__G__MathFitLN_auto_ptrlEROOTcLcLMathcLcLMinimizergR;
extern G__linked_taginfo G__G__MathFitLN_auto_ptrlEROOTcLcLMathcLcLIBaseFunctionMultiDimgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLLogLikelihoodFCNlEROOTcLcLMathcLcLIBaseFunctionMultiDimgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLLogLikelihoodFCNlEROOTcLcLMathcLcLIGradientFunctionMultiDimgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLPoissonLikelihoodFCNlEROOTcLcLMathcLcLIBaseFunctionMultiDimgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLPoissonLikelihoodFCNlEROOTcLcLMathcLcLIGradientFunctionMultiDimgR;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLProxyListBox;
extern G__linked_taginfo G__G__MathFitLN_ROOTcLcLFitcLcLSparseData;

/* STUB derived class for protected member access */
typedef ROOT::Fit::Chi2FCN<ROOT::Math::IBaseFunctionMultiDim> G__ROOTcLcLFitcLcLChi2FCNlEROOTcLcLMathcLcLIBaseFunctionMultiDimgR;
typedef ROOT::Fit::Chi2FCN<ROOT::Math::IGradientFunctionMultiDim> G__ROOTcLcLFitcLcLChi2FCNlEROOTcLcLMathcLcLIGradientFunctionMultiDimgR;
typedef vector<ROOT::Fit::ParameterSettings,allocator<ROOT::Fit::ParameterSettings> > G__vectorlEROOTcLcLFitcLcLParameterSettingscOallocatorlEROOTcLcLFitcLcLParameterSettingsgRsPgR;
typedef ROOT::Fit::LogLikelihoodFCN<ROOT::Math::IBaseFunctionMultiDim> G__ROOTcLcLFitcLcLLogLikelihoodFCNlEROOTcLcLMathcLcLIBaseFunctionMultiDimgR;
typedef ROOT::Fit::LogLikelihoodFCN<ROOT::Math::IGradientFunctionMultiDim> G__ROOTcLcLFitcLcLLogLikelihoodFCNlEROOTcLcLMathcLcLIGradientFunctionMultiDimgR;
typedef ROOT::Fit::PoissonLikelihoodFCN<ROOT::Math::IBaseFunctionMultiDim> G__ROOTcLcLFitcLcLPoissonLikelihoodFCNlEROOTcLcLMathcLcLIBaseFunctionMultiDimgR;
typedef ROOT::Fit::PoissonLikelihoodFCN<ROOT::Math::IGradientFunctionMultiDim> G__ROOTcLcLFitcLcLPoissonLikelihoodFCNlEROOTcLcLMathcLcLIGradientFunctionMultiDimgR;
