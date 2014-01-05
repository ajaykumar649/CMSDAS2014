// @(#)root/mathmore:$Id: LinkDef_RootFinding.h 22522 2008-03-07 16:07:51Z moneta $
// Authors: L. Moneta, A. Zsenei   08/2005 

#ifdef __CINT__


#pragma link C++ namespace ROOT::Math::Roots;

#pragma link C++ class ROOT::Math::GSLRootFinder+;
#pragma link C++ class ROOT::Math::GSLRootFinderDeriv+;

#pragma link C++ class ROOT::Math::Roots::Bisection+;
#pragma link C++ class ROOT::Math::Roots::Brent+;
#pragma link C++ class ROOT::Math::Roots::FalsePos+;
#pragma link C++ class ROOT::Math::Roots::Newton+;
#pragma link C++ class ROOT::Math::Roots::Secant+;
#pragma link C++ class ROOT::Math::Roots::Steffenson+;

#endif
