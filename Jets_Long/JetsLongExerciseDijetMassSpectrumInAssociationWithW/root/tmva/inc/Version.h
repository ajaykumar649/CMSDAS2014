// @(#)root/tmva $Id: Version.h 37512 2010-12-10 14:19:02Z stelzer $   
// Author: Andreas Hoecker, Peter Speckmayer, Joerg Stelzer, Jan Therhaag, Eckhard von Toerne, Helge Voss

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : Version                                                               *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Current TMVA Version - filled automatically during cvs tagging            *
 *      by python script development/tmvaTag.py                                   *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Andreas Hoecker    <Andreas.Hocker@cern.ch>      - CERN, Switzerland      *                      
 *      Peter Speckmayer   <Peter.Speckmayer@cern.ch>    - CERN, Switzerland      *                      
 *      Joerg Stelzer      <Joerg.Stelzer@cern.ch>       - CERN, Switzerland      *                      
 *      Jan Therhaag       <therhaag@physik.uni-bonn.de> - U of Bonn, Germany     *                      
 *      Eckhard v. Toerne  <evt@uni-bonn.de>             - U of Bonn, Germany     *                      
 *      Helge Voss         <Helge.Voss@cern.ch>          - MPI-K Heidelberg, GER  *                      
 *                                                                                *                      
 * Copyright (c) 2005-2010,                                                       *
 * Regents of CERN (Switzerland),                                                 *
 * University of Bonn (Germany),                                                  *
 * University of Heidelberg (Germany),                                            *
 * University of Victoria (Canada)                                                *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_Version
#define ROOT_TMVA_Version

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Version                                                              //
//                                                                      //
// Automatically filled by cvs tagger: development/tmvaTag.py           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#define TMVA_RELEASE      "4.1.0"
#define TMVA_RELEASE_DATE "Dec 10, 2010"
#define TMVA_RELEASE_TIME "15:15:00"
#define TMVA_VERSION_CODE 262400
#define TMVA_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#endif

