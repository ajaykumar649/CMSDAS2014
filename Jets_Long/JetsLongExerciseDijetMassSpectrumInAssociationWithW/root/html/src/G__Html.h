/********************************************************************
* html/src/G__Html.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error html/src/G__Html.h/C is only for compilation. Abort cint.
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
extern void G__cpp_setup_tagtableG__Html();
extern void G__cpp_setup_inheritanceG__Html();
extern void G__cpp_setup_typetableG__Html();
extern void G__cpp_setup_memvarG__Html();
extern void G__cpp_setup_globalG__Html();
extern void G__cpp_setup_memfuncG__Html();
extern void G__cpp_setup_funcG__Html();
extern void G__set_cpp_environmentG__Html();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "TClassDocOutput.h"
#include "TDocDirective.h"
#include "TDocInfo.h"
#include "TDocOutput.h"
#include "TDocParser.h"
#include "THtml.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__G__HtmlLN_TClass;
extern G__linked_taginfo G__G__HtmlLN_TBuffer;
extern G__linked_taginfo G__G__HtmlLN_TMemberInspector;
extern G__linked_taginfo G__G__HtmlLN_TObject;
extern G__linked_taginfo G__G__HtmlLN_TNamed;
extern G__linked_taginfo G__G__HtmlLN_TString;
extern G__linked_taginfo G__G__HtmlLN_basic_istreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__HtmlLN_basic_ostreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__HtmlLN_basic_ifstreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__G__HtmlLN_string;
extern G__linked_taginfo G__G__HtmlLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__G__HtmlLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__HtmlLN_TList;
extern G__linked_taginfo G__G__HtmlLN_TMethod;
extern G__linked_taginfo G__G__HtmlLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__G__HtmlLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__G__HtmlLN_TSubString;
extern G__linked_taginfo G__G__HtmlLN_TVirtualMutex;
extern G__linked_taginfo G__G__HtmlLN_TIter;
extern G__linked_taginfo G__G__HtmlLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR;
extern G__linked_taginfo G__G__HtmlLN_THashTable;
extern G__linked_taginfo G__G__HtmlLN_THashList;
extern G__linked_taginfo G__G__HtmlLN_TBaseClass;
extern G__linked_taginfo G__G__HtmlLN_TClassDocOutput;
extern G__linked_taginfo G__G__HtmlLN_TDocOutput;
extern G__linked_taginfo G__G__HtmlLN_THtml;
extern G__linked_taginfo G__G__HtmlLN_TDocMethodWrapper;
extern G__linked_taginfo G__G__HtmlLN_TDocParser;
extern G__linked_taginfo G__G__HtmlLN_TDocParsercLcLEDocContext;
extern G__linked_taginfo G__G__HtmlLN_TDocParsercLcLESourceInfo;
extern G__linked_taginfo G__G__HtmlLN_TDocParsercLcLEAccess;
extern G__linked_taginfo G__G__HtmlLN_TDocParsercLcLEParseContext;
extern G__linked_taginfo G__G__HtmlLN_TDocParsercLcLEParseContextFlag;
extern G__linked_taginfo G__G__HtmlLN_maplEstringcOintcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOintgRsPgRsPgR;
extern G__linked_taginfo G__G__HtmlLN_listlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR;
extern G__linked_taginfo G__G__HtmlLN_TDocParsercLcLdA;
extern G__linked_taginfo G__G__HtmlLN_setlEunsignedsPintcOlesslEunsignedsPintgRcOallocatorlEunsignedsPintgRsPgR;
extern G__linked_taginfo G__G__HtmlLN_setlEstringcOlesslEstringgRcOallocatorlEstringgRsPgR;
extern G__linked_taginfo G__G__HtmlLN_TDataMember;
extern G__linked_taginfo G__G__HtmlLN_TDataType;
extern G__linked_taginfo G__G__HtmlLN_TGClient;
extern G__linked_taginfo G__G__HtmlLN_TModuleDocInfo;
extern G__linked_taginfo G__G__HtmlLN_TVirtualPad;
extern G__linked_taginfo G__G__HtmlLN_TDocOutputcLcLEGraphvizTool;
extern G__linked_taginfo G__G__HtmlLN_TDocOutputcLcLEFileType;
extern G__linked_taginfo G__G__HtmlLN_TClassDocOutputcLcLETraverse;
extern G__linked_taginfo G__G__HtmlLN_TLatex;
extern G__linked_taginfo G__G__HtmlLN_TMacro;
extern G__linked_taginfo G__G__HtmlLN_TDocDirective;
extern G__linked_taginfo G__G__HtmlLN_TDocHtmlDirective;
extern G__linked_taginfo G__G__HtmlLN_TDocMacroDirective;
extern G__linked_taginfo G__G__HtmlLN_TDocLatexDirective;
extern G__linked_taginfo G__G__HtmlLN_TDictionary;
extern G__linked_taginfo G__G__HtmlLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR;
extern G__linked_taginfo G__G__HtmlLN_TClassDocInfo;
extern G__linked_taginfo G__G__HtmlLN_TLibraryDocInfo;
extern G__linked_taginfo G__G__HtmlLN_TExMap;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLTHelperBase;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLTFileSysEntry;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLTModuleDefinition;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLTFileDefinition;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLTPathDefinition;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLTFileSysDir;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLTFileSysDB;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLTFileSysRoot;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLPathInfo_t;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLEConvertOutput;
extern G__linked_taginfo G__G__HtmlLN_maplETClassmUcOintcOlesslETClassmUgRcOallocatorlEpairlETClassmUsPconstcOintgRsPgRsPgR;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLDocSyntax_t;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLLinkInfo_t;
extern G__linked_taginfo G__G__HtmlLN_maplEstringcOTStringcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTStringgRsPgRsPgR;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLOutputStyle_t;
extern G__linked_taginfo G__G__HtmlLN_THtmlcLcLDocEntityInfo_t;

/* STUB derived class for protected member access */
