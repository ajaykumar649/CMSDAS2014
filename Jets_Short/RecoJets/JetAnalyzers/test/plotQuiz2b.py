#!/usr/bin/env python


# ===================================================
#             plotDataMCQuick.py
#
# ===================================================
#    Compares the data and MC distributions from
#    a simple substructure analysis of dijet data
# ===================================================

from ROOT import *

from optparse import OptionParser


parser = OptionParser()

parser.add_option('--inputFile1', metavar='F', type='string', action='store',
                  default='PFJetPlotsExample_MC.root',
                  dest='inputFile1',
                  help='Input file 1')

parser.add_option('--inputFile2', metavar='F', type='string', action='store',
                  default='CAJetPlotsExample_MC.root',
                  dest='inputFile2',
                  help='Input file 2')

parser.add_option('--inputFile3', metavar='F', type='string', action='store',
                  default='CAPrunedJetPlotsExample_MC.root',
                  dest='inputFile3',
                  help='Input file 3')

parser.add_option('--outname', metavar='O', type='string', action='store',
                  default='ComparisonPFBasic.root',
                  dest='outname',
                  help='Output name')


#parser.add_option('--dir', metavar='D', type='string', action='store',
#                  default='pf/',
#                  dest='dir',
#                  help='directory to find plots')


(options, args) = parser.parse_args()

argv = []

#gROOT.Macro("rootlogon.C")

f1 = TFile(options.inputFile1)
f2 = TFile(options.inputFile2)
f3 = TFile(options.inputFile3)

hists = [
    'JetArea',	
    ]

outhists = []
stacks = []
canvs = []

for ihist in hists:
    h1 = f1.Get(ihist)
    h2 = f2.Get(ihist)
    h3 = f3.Get(ihist)
    hs = THStack( h1.GetName(), h1.GetTitle() )
    outhists.append( [h1,h2,h3] )
    stacks.append( hs )

    h2.Scale(1.0 / h2.Integral() * h1.Integral() )
    h3.Scale(1.0 / h3.Integral() * h1.Integral() ) 
    h1.SetLineColor(2)
    h1.SetFillStyle(3005);
    h1.SetFillColor(2)
    h1.SetLineWidth(2)  
    h2.SetLineWidth(2) 
    h2.SetLineColor(1)
    h3.SetLineWidth(2) 
    h3.SetLineColor(4) 

    hs.Add( h3, 'hist' )
    hs.Add( h2, 'hist' )
    hs.Add( h1, 'hist' )

    c = TCanvas('c' + ihist, 'c' + ihist)
    hs.Draw('nostack')
    canvs.append(c)
