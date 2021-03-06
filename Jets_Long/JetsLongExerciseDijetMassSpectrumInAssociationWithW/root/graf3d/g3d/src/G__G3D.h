/********************************************************************
* graf3d/g3d/src/G__G3D.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error graf3d/g3d/src/G__G3D.h/C is only for compilation. Abort cint.
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
extern void G__cpp_setup_tagtableG__G3D();
extern void G__cpp_setup_inheritanceG__G3D();
extern void G__cpp_setup_typetableG__G3D();
extern void G__cpp_setup_memvarG__G3D();
extern void G__cpp_setup_globalG__G3D();
extern void G__cpp_setup_memfuncG__G3D();
extern void G__cpp_setup_funcG__G3D();
extern void G__set_cpp_environmentG__G3D();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "TAxis3D.h"
#include "TBRIK.h"
#include "TCONE.h"
#include "TCONS.h"
#include "TCTUB.h"
#include "TELTU.h"
#include "TGTRA.h"
#include "TGeometry.h"
#include "THYPE.h"
#include "THelix.h"
#include "TMarker3DBox.h"
#include "TMaterial.h"
#include "TMixture.h"
#include "TNode.h"
#include "TNodeDiv.h"
#include "TPARA.h"
#include "TPCON.h"
#include "TPGON.h"
#include "TPointSet3D.h"
#include "TPoints3DABC.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TRotMatrix.h"
#include "TSPHE.h"
#include "TShape.h"
#include "TTRAP.h"
#include "TTRD1.h"
#include "TTRD2.h"
#include "TTUBE.h"
#include "TTUBS.h"
#include "TView3D.h"
#include "TXTRU.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__G__G3DLN_TClass;
extern G__linked_taginfo G__G__G3DLN_TBuffer;
extern G__linked_taginfo G__G__G3DLN_TMemberInspector;
extern G__linked_taginfo G__G__G3DLN_TObject;
extern G__linked_taginfo G__G__G3DLN_TNamed;
extern G__linked_taginfo G__G__G3DLN_TString;
extern G__linked_taginfo G__G__G3DLN_basic_ostreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__G3DLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__G__G3DLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__G3DLN_TList;
extern G__linked_taginfo G__G__G3DLN_TBrowser;
extern G__linked_taginfo G__G__G3DLN_TObjArray;
extern G__linked_taginfo G__G__G3DLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__G__G3DLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__G3DLN_THashList;
extern G__linked_taginfo G__G__G3DLN_TAxis;
extern G__linked_taginfo G__G__G3DLN_TGaxis;
extern G__linked_taginfo G__G__G3DLN_TVirtualPad;
extern G__linked_taginfo G__G__G3DLN_TView;
extern G__linked_taginfo G__G__G3DLN_TAxis3D;
extern G__linked_taginfo G__G__G3DLN_TAttFill;
extern G__linked_taginfo G__G__G3DLN_TMaterial;
extern G__linked_taginfo G__G__G3DLN_TAttLine;
extern G__linked_taginfo G__G__G3DLN_TAtt3D;
extern G__linked_taginfo G__G__G3DLN_TBuffer3D;
extern G__linked_taginfo G__G__G3DLN_TNode;
extern G__linked_taginfo G__G__G3DLN_TShape;
extern G__linked_taginfo G__G__G3DLN_TBRIK;
extern G__linked_taginfo G__G__G3DLN_TTUBE;
extern G__linked_taginfo G__G__G3DLN_TCONE;
extern G__linked_taginfo G__G__G3DLN_TTUBS;
extern G__linked_taginfo G__G__G3DLN_TCONS;
extern G__linked_taginfo G__G__G3DLN_TCTUB;
extern G__linked_taginfo G__G__G3DLN_TELTU;
extern G__linked_taginfo G__G__G3DLN_TGTRA;
extern G__linked_taginfo G__G__G3DLN_TCollection;
extern G__linked_taginfo G__G__G3DLN_TSeqCollection;
extern G__linked_taginfo G__G__G3DLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR;
extern G__linked_taginfo G__G__G3DLN_TRotMatrix;
extern G__linked_taginfo G__G__G3DLN_TGeometry;
extern G__linked_taginfo G__G__G3DLN_THYPE;
extern G__linked_taginfo G__G__G3DLN_TPolyLine3D;
extern G__linked_taginfo G__G__G3DLN_TRotMatrixcLcLdA;
extern G__linked_taginfo G__G__G3DLN_EHelixRangeType;
extern G__linked_taginfo G__G__G3DLN_THelix;
extern G__linked_taginfo G__G__G3DLN_TH1;
extern G__linked_taginfo G__G__G3DLN_TMarker3DBox;
extern G__linked_taginfo G__G__G3DLN_TMarker3DBoxcLcLdA;
extern G__linked_taginfo G__G__G3DLN_TMixture;
extern G__linked_taginfo G__G__G3DLN_TNodecLcLdA;
extern G__linked_taginfo G__G__G3DLN_TNodeDiv;
extern G__linked_taginfo G__G__G3DLN_TPARA;
extern G__linked_taginfo G__G__G3DLN_TPCON;
extern G__linked_taginfo G__G__G3DLN_TPGON;
extern G__linked_taginfo G__G__G3DLN_TAttMarker;
extern G__linked_taginfo G__G__G3DLN_TPolyMarker3D;
extern G__linked_taginfo G__G__G3DLN_TAttBBox;
extern G__linked_taginfo G__G__G3DLN_TRefArray;
extern G__linked_taginfo G__G__G3DLN_TPointSet3D;
extern G__linked_taginfo G__G__G3DLN_TPoints3DABC;
extern G__linked_taginfo G__G__G3DLN_TSPHE;
extern G__linked_taginfo G__G__G3DLN_TTRAP;
extern G__linked_taginfo G__G__G3DLN_TTRD1;
extern G__linked_taginfo G__G__G3DLN_TTRD2;
extern G__linked_taginfo G__G__G3DLN_TView3D;
extern G__linked_taginfo G__G__G3DLN_TView3DcLcLdA;
extern G__linked_taginfo G__G__G3DLN_TXTRU;
extern G__linked_taginfo G__G__G3DLN_TXTRUcLcLEXYChecked;
extern G__linked_taginfo G__G__G3DLN_TXTRUcLcLEZChecked;

/* STUB derived class for protected member access */
